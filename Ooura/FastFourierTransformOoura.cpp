/*
 
 This file is a part of Grizzly, a modern C++ library for digital signal
 processing. See https://github.com/dsperados/grizzly for more information.
 
 Copyright (C) 2016 Dsperados <info@dsperados.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 --------------------------------------------------------------------
 
 If you would like to use Grizzly for commercial or closed-source
 purposes, please contact us for a commercial license.
 
 */

#include <algorithm>
#include <dsperados/math/interleave.hpp>

#include "FastFourierTransformOoura.hpp"
#include "fftsg.hpp"

using namespace std;

namespace dsp
{
    FastFourierTransformOoura::FastFourierTransformOoura(size_t size) :
        FastFourierTransformBase(size),
        data(size),
        ip(static_cast<size_t>(2 + sqrt(size))),
        w(size / 2 - 1),
        dataComplex(size * 2)
    {
        // Ensure ip[0] is zero, otherwise the zero and cosines won't be generated
        ip[0] = 0;
    }

    void FastFourierTransformOoura::forward(const float* input, float* real, float* imaginary)
    {
        data.assign(input, input + size);
        
        rdft(static_cast<int>(size), 1, data.data(), ip.data(), w.data());
        
        for (auto i = 0; i < size / 2; ++i)
        {
            real[i] = data[i * 2];
            imaginary[i] = -data[i * 2 + 1];
        }
        
        real[size / 2] = - imaginary[0];
        imaginary[0] = 0;
        imaginary[size / 2] = 0;
    }
    
    void FastFourierTransformOoura::forward(const double* input, double* real, double* imaginary)
    {
        data.assign(input, input + size);
        
        rdft(static_cast<int>(size), 1, data.data(), ip.data(), w.data());
        
        for (auto i = 0; i < size / 2; ++i)
        {
            real[i] = data[i * 2];
            imaginary[i] = -data[i * 2 + 1]; // Flip imaginary axis, otherwise Ooura will invert the signal
        }
        
        real[size / 2] = - imaginary[0];
        imaginary[0] = 0;
        imaginary[size / 2] = 0;
    }
    
    void FastFourierTransformOoura::inverse(const float* real, const float* imaginary, float* output)
    {
        for (auto i = 0; i < size / 2; ++i)
        {
            data[i * 2] = real[i];
            data[i * 2 + 1] = -imaginary[i]; // Flip imaginary axis, otherwise Ooura will invert the signal
        }
        
        data[1] = real[size / 2];
        
        rdft(static_cast<int>(size), -1, data.data(), ip.data(), w.data());
        
        move(data.begin(), data.end(), output);
        
        float factor = 2.0 / size;
        std::transform(output, output + size, output, [&](const float& x){ return x * factor; });
    }
    
    void FastFourierTransformOoura::inverse(const double* real, const double* imaginary, double* output)
    {
        for (auto i = 0; i < size / 2; ++i)
        {
            data[i * 2] = real[i];
            data[i * 2 + 1] = -imaginary[i];
        }
        
        data[1] = real[size / 2];
        
        rdft(static_cast<int>(size), -1, data.data(), ip.data(), w.data());
        
        move(data.begin(), data.end(), output);
        
        float factor = 2.0 / size;
        std::transform(output, output + size, output, [&](const double& x){ return x * factor; });
    }
    
    void FastFourierTransformOoura::forwardComplex(const float* inReal, const float* inImaginary, float* outReal, float* outImaginary)
    {
        math::interleave(inReal, inReal + size, inImaginary, dataComplex.begin());
        
        cdft(static_cast<int>(size * 2), 1, dataComplex.data(), ip.data(), w.data());
        
        math::deinterleave(dataComplex.begin(), dataComplex.end(), outReal, outImaginary);
        std::transform(outImaginary, outImaginary + size, outImaginary, std::negate<>());
    }
    
    void FastFourierTransformOoura::forwardComplex(const double* inReal, const double* inImaginary, double* outReal, double* outImaginary)
    {
        math::interleave(inReal, inReal + size, inImaginary, dataComplex.begin());
        
        cdft(static_cast<int>(size * 2), 1, dataComplex.data(), ip.data(), w.data());
        
        math::deinterleave(dataComplex.begin(), dataComplex.end(), outReal, outImaginary);
        std::transform(outImaginary, outImaginary + size, outImaginary, std::negate<>());
    }
    
    void FastFourierTransformOoura::inverseComplex(const float* inReal, const float* inImaginary, float* outReal, float* outImaginary)
    {
        math::interleave(inReal, inReal + size, inImaginary, dataComplex.begin());
        
        cdft(static_cast<int>(size * 2), -1, dataComplex.data(), ip.data(), w.data());
        
        const float factor = 1.0 / size;
        std::transform(dataComplex.begin(), dataComplex.end(), dataComplex.begin(), [&](const double& x){ return x * factor; });
        
        math::deinterleave(dataComplex.begin(), dataComplex.end(), outReal, outImaginary);
        std::transform(outReal, outReal + size, outReal, std::negate<>());
    }
    
    void FastFourierTransformOoura::inverseComplex(const double* inReal, const double* inImaginary, double* outReal, double* outImaginary)
    {
        math::interleave(inReal, inReal + size, inImaginary, dataComplex.begin());
        
        cdft(static_cast<int>(size * 2), -1, dataComplex.data(), ip.data(), w.data());
        
        const double factor = 1.0 / size;
        std::transform(dataComplex.begin(), dataComplex.end(), dataComplex.begin(), [&](const double& x){ return x * factor; });
        
        math::deinterleave(dataComplex.begin(), dataComplex.end(), outReal, outImaginary);
        std::transform(outReal, outReal + size, outReal, std::negate<>());
    }
}
