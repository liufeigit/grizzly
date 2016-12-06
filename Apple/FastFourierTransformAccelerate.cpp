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

#include <dsperados/math/interleave.hpp>
#include <stdexcept>

#include "FastFourierTransformAccelerate.hpp"

using namespace math;
using namespace std;

namespace dsp
{
    FastFourierTransformAccelerate::FastFourierTransformAccelerate(size_t size) :
        FastFourierTransformBase(size),
        evenFloat(size / 2),
        oddFloat(size / 2),
        evenDouble(size / 2),
        oddDouble(size / 2)
    {
        floatSetup.forward = vDSP_DFT_zrop_CreateSetup(nullptr, size, vDSP_DFT_FORWARD);
        floatSetup.inverse = vDSP_DFT_zrop_CreateSetup(nullptr, size, vDSP_DFT_INVERSE);
        
        floatComplexSetup.forward = vDSP_DFT_zop_CreateSetup(nullptr, size, vDSP_DFT_FORWARD);
        floatComplexSetup.inverse = vDSP_DFT_zop_CreateSetup(nullptr, size, vDSP_DFT_INVERSE);
        
        doubleSetup.forward = vDSP_DFT_zrop_CreateSetupD(nullptr, size, vDSP_DFT_FORWARD);
        doubleSetup.inverse = vDSP_DFT_zrop_CreateSetupD(nullptr, size, vDSP_DFT_INVERSE);
        
        doubleComplexSetup.forward = vDSP_DFT_zop_CreateSetupD(nullptr, size, vDSP_DFT_FORWARD);
        doubleComplexSetup.inverse = vDSP_DFT_zop_CreateSetupD(nullptr, size, vDSP_DFT_INVERSE);
        
        if (!floatSetup.forward || !floatSetup.inverse || !floatComplexSetup.forward || !floatComplexSetup.inverse ||
            !doubleSetup.forward || !doubleSetup.inverse || !doubleComplexSetup.forward || !doubleComplexSetup.inverse)
        {
            throw runtime_error("Could not setup vDSP FFT");
        }
    }
    
    void FastFourierTransformAccelerate::forward(const float* input, float* real, float* imaginary)
    {
        // Split the input signal to even and odd arrays
        math::deinterleave(input, input + size, evenFloat.begin(), oddFloat.begin());

        // Do the transform
        vDSP_DFT_Execute(floatSetup.forward, evenFloat.data(), oddFloat.data(), real, imaginary);

        // In the forward direction, the scale is 2 (for some reason), so scale back by a half
        // Probably because both the negative and positive frequencies get summed, or something. The complex-to-complex
        // DFT in Accelerate (see function below) doesn't need scaling).
        std::transform(real, real + (size / 2 + 1), real, [](const float& lhs) { return lhs * 0.5f; });
        std::transform(imaginary, imaginary + (size / 2 + 1), imaginary, [](const float& lhs) { return lhs * 0.5f; });

        // Because the Im[0] and Im[Nyquist] are always 0, vDSP stores the Re[Nyquist]
        // in Im[0], so that it can return one complex number less. Imho, this only makes
        // things more complicated and hardly seems worth the hassle, but there you have it.
        // Therefore, we copy Im[0] to Re[Nyquist]...
        real[size / 2] = imaginary[0];

        // ... And set Im[0] and Im[Nyquist] to 0, which they always are
        imaginary[0] = 0.0f;
        imaginary[size / 2] = 0.0f;
    }
    
    void FastFourierTransformAccelerate::forward(const double* input, double* real, double* imaginary)
    {
        // Split the input signal to even and odd arrays
        math::deinterleave(input, input + size, evenDouble.begin(), oddDouble.begin());
        
        // Do the transform
        vDSP_DFT_ExecuteD(doubleSetup.forward, evenDouble.data(), oddDouble.data(), real, imaginary);
        
        // In the forward direction, the scale is 2 (for some reason), so scale back by a half
        // Probably because both the negative and positive frequencies get summed, or something. The complex-to-complex
        // DFT in Accelerate (see function below) doesn't need scaling).
        std::transform(real, real + (size / 2 + 1), real, [](const float& lhs) { return lhs * 0.5f; });
        std::transform(imaginary, imaginary + (size / 2 + 1), imaginary, [](const float& lhs) { return lhs * 0.5f; });
        
        // Because the Im[0] and Im[Nyquist] are always 0, vDSP stores the Re[Nyquist]
        // in Im[0], so that it can return one complex number less. Imho, this only makes
        // things more complicated and hardly seems worth the hassle, but there you have it.
        // Therefore, we copy Im[0] to Re[Nyquist]...
        real[size / 2] = imaginary[0];
        
        // ... And set Im[0] and Im[Nyquist] to 0, which they always are
        imaginary[0] = 0.0f;
        imaginary[size / 2] = 0.0f;
    }
    
    void FastFourierTransformAccelerate::inverse(const float* real, const float* imaginary, float* output)
    {
        // Copy the input reals and imaginaries, so that we can change the format around to
        // the way vDSP accepts it
        vector<float> real_(real, real + size / 2 + 1);
        vector<float> imaginary_(imaginary, imaginary + size / 2 + 1);

        // Re[Nyquist] is supposed to be stored in Im[0] for vDSP
        imaginary_[0] = real[size / 2];

        // Do the transform
        vDSP_DFT_Execute(floatSetup.inverse, real_.data(), imaginary_.data(), real_.data(), imaginary_.data());

        // Combine the even and odd output signals into one interleaved output signal
        math::interleave(real_.begin(), real_.end(), imaginary_.begin(), output);

        // For inverse DFT, the scaling is Size, so scale back by multiplying with its reciprocal
        const float factor = 1.0 / size;
        std::transform(output, output + size, output, [&](const float& lhs) { return lhs * factor; });
    }
    
    void FastFourierTransformAccelerate::inverse(const double* real, const double* imaginary, double* output)
    {
        // Copy the input reals and imaginaries, so that we can change the format around to
        // the way vDSP accepts it
        vector<double> real_(real, real + (size / 2 + 1));
        vector<double> imaginary_(imaginary, imaginary + (size / 2 + 1));
        
        // Re[Nyquist] is supposed to be stored in Im[0] for vDSP
        imaginary_[0] = real[size / 2];
        
        // Do the transform
        vDSP_DFT_ExecuteD(doubleSetup.inverse, real_.data(), imaginary_.data(), real_.data(), imaginary_.data());
        
        // Combine the even and odd output signals into one interleaved output signal
        math::interleave(real_.begin(), real_.end(), imaginary_.begin(), output);
        
        // For inverse DFT, the scaling is Size, so scale back by multiplying with its reciprocal
        const double factor = 1.0 / size;
        std::transform(output, output + size, output, [&](const double& lhs) { return lhs * factor; });
    }
    
    void FastFourierTransformAccelerate::forwardComplex(const float* inReal, const float* inImaginary, float* outReal, float* outImaginary)
    {
        // Do the transform
        vDSP_DFT_Execute(floatComplexSetup.forward, inReal, inImaginary, outReal, outImaginary);
    }
    
    void FastFourierTransformAccelerate::forwardComplex(const double* inReal, const double* inImaginary, double* outReal, double* outImaginary)
    {
        // Do the transform
        vDSP_DFT_ExecuteD(doubleComplexSetup.forward, inReal, inImaginary, outReal, outImaginary);
    }
    
    void FastFourierTransformAccelerate::inverseComplex(const float* inReal, const float* inImaginary, float* outReal, float* outImaginary)
    {
        // Do the transform
        vDSP_DFT_Execute(floatComplexSetup.inverse, inReal, inImaginary, outReal, outImaginary);
        
        // For inverse DFT, the scaling is Size, so scale back by multiplying with its reciprocal
        const float factor = 1.0 / size;
        std::transform(outReal, outReal + size, outReal, [&](const float& lhs) { return lhs * factor; });
        std::transform(outImaginary, outImaginary + size, outImaginary, [&](const float& lhs) { return lhs * factor; });
    }
    
    void FastFourierTransformAccelerate::inverseComplex(const double* inReal, const double* inImaginary, double* outReal, double* outImaginary)
    {
        // Do the transform
        vDSP_DFT_ExecuteD(doubleComplexSetup.inverse, inReal, inImaginary, outReal, outImaginary);
        
        // For inverse DFT, the scaling is Size, so scale back by multiplying with its reciprocal
        const double factor = 1.0 / size;
        std::transform(outReal, outReal + size, outReal, [&](const float& lhs) { return lhs * factor; });
        std::transform(outImaginary, outImaginary + size, outImaginary, [&](const float& lhs) { return lhs * factor; });
    }
}
