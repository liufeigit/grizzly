//
//  FastFourierTransformOoura.cpp
//  bear-dsp
//
//  Created by Stijn on 09/06/2016.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#include <algorithm>
#include <dsperados/math/interleave.hpp>

#include "../FastFourierTransform.hpp"
#include "fftsg.h"

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
        ip.front() = 0;
    }

    void FastFourierTransformOoura::doForward(const float* input, float* real, float* imaginary)
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
    
    void FastFourierTransformOoura::doForward(const double* input, double* real, double* imaginary)
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
    
    void FastFourierTransformOoura::doInverse(const float* real, const float* imaginary, float* output)
    {
        for (auto i = 0; i < size / 2; ++i)
        {
            data[i * 2] = real[i];
            data[i * 2 + 1] = imaginary[i];
        }
        
        data[1] = real[size / 2];
        
        rdft(static_cast<int>(size), -1, data.data(), ip.data(), w.data());
        
        move(data.begin(), data.end(), output);
        
        float factor = 2.0 / size;
        std::transform(output, output + size, output, [&](const float& x){ return x * factor; });
    }
    
    void FastFourierTransformOoura::doInverse(const double* real, const double* imaginary, double* output)
    {
        for (auto i = 0; i < size / 2; ++i)
        {
            data[i * 2] = real[i];
            data[i * 2 + 1] = imaginary[i];
        }
        
        data[1] = real[size / 2];
        
        rdft(static_cast<int>(size), -1, data.data(), ip.data(), w.data());
        
        move(data.begin(), data.end(), output);
        
        float factor = 2.0 / size;
        std::transform(output, output + size, output, [&](const double& x){ return x * factor; });
    }
    
    void FastFourierTransformOoura::doForwardComplex(const vector<float>& inReal, const vector<float>& inImaginary, vector<float>& outReal, vector<float>& outImaginary)
    {
        math::interleave(inReal.begin(), inReal.end(), inImaginary.begin(), dataComplex.begin());
        
        cdft(static_cast<int>(size * 2), 1, dataComplex.data(), ip.data(), w.data());

        math::deinterleave(dataComplex.begin(), dataComplex.end(), outReal.begin(), outImaginary.end());
    }
    
    void FastFourierTransformOoura::doForwardComplex(const vector<double>& inReal, const vector<double>& inImaginary, vector<double>& outReal, vector<double>& outImaginary)
    {
        math::interleave(inReal.begin(), inReal.end(), inImaginary.begin(), dataComplex.begin());
        
        cdft(static_cast<int>(size * 2), 1, dataComplex.data(), ip.data(), w.data());
        
        math::deinterleave(dataComplex.begin(), dataComplex.end(), outReal.begin(), outImaginary.end());
    }
    
    void FastFourierTransformOoura::doInverseComplex(const vector<float>& inReal, const vector<float>& inImaginary, vector<float>& outReal, vector<float>& outImaginary)
    {
        math::interleave(inReal.begin(), inReal.end(), inImaginary.begin(), dataComplex.begin());
        
        cdft(static_cast<int>(size * 2), -1, dataComplex.data(), ip.data(), w.data());
        
        const float factor = 1.0 / size;
        std::transform(dataComplex.begin(), dataComplex.end(), dataComplex.begin(), [&](const double& x){ return x * factor; });
        
        math::deinterleave(dataComplex.begin(), dataComplex.end(), outReal.begin(), outImaginary.end());
    }
    
    void FastFourierTransformOoura::doInverseComplex(const vector<double>& inReal, const vector<double>& inImaginary, vector<double>& outReal, vector<double>& outImaginary)
    {
        math::interleave(inReal.begin(), inReal.end(), inImaginary.begin(), dataComplex.begin());
        
        cdft(static_cast<int>(size * 2), -1, dataComplex.data(), ip.data(), w.data());
        
        const double factor = 1.0 / size;
        std::transform(dataComplex.begin(), dataComplex.end(), dataComplex.begin(), [&](const double& x){ return x * factor; });
        
        math::deinterleave(dataComplex.begin(), dataComplex.end(), outReal.begin(), outImaginary.end());
    }
}
