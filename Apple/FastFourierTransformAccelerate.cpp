//
//  FastFourierTransformAccelerate.cpp
//  bear-dsp
//
//  Created by Stijn on 09/06/2016.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#include <stdexcept>

#include "../FastFourierTransform.hpp"
#include "../Parallel.hpp"

using namespace gsl;
using namespace std;

namespace bear::dsp
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
    
    void FastFourierTransformAccelerate::doForward(span<const float> input, span<float> real, span<float> imaginary)
    {
        // Split the input signal to even and odd arrays
        deinterleave(input, span<float>(evenFloat), span<float>(oddFloat));

        // Do the transform
        vDSP_DFT_Execute(floatSetup.forward, evenFloat.data(), oddFloat.data(), real.data(), imaginary.data());

        // In the forward direction, the scale is 2 (for some reason), so scale back by a half
        // Probably because both the negative and positive frequencies get summed, or something. The complex-to-complex
        // DFT in Accelerate (see function below) doesn't need scaling).
        const float factor = 0.5f;
        multiply(span<const float>(real), factor, real);
        multiply(span<const float>(imaginary), factor, imaginary);

        // Because the Im[0] and Im[Nyquist] are always 0, vDSP stores the Re[Nyquist]
        // in Im[0], so that it can return one complex number less. Imho, this only makes
        // things more complicated and hardly seems worth the hassle, but there you have it.
        // Therefore, we copy Im[0] to Re[Nyquist]...
        real[size / 2] = imaginary[0];

        // ... And set Im[0] and Im[Nyquist] to 0, which they always are
        imaginary[0] = 0.0f;
        imaginary[size / 2] = 0.0f;
    }
    
    void FastFourierTransformAccelerate::doForward(span<const double> input, span<double> real, span<double> imaginary)
    {
        // Split the input signal to even and odd arrays
        deinterleave(input, span<double>(evenDouble), span<double>(oddDouble));
        
        // Do the transform
        vDSP_DFT_ExecuteD(doubleSetup.forward, evenDouble.data(), oddDouble.data(), real.data(), imaginary.data());
        
        // In the forward direction, the scale is 2 (for some reason), so scale back by a half
        // Probably because both the negative and positive frequencies get summed, or something. The complex-to-complex
        // DFT in Accelerate (see function below) doesn't need scaling).
        const double factor = 0.5f;
        multiply(span<const double>(real), factor, real);
        multiply(span<const double>(imaginary), factor, imaginary);
        
        // Because the Im[0] and Im[Nyquist] are always 0, vDSP stores the Re[Nyquist]
        // in Im[0], so that it can return one complex number less. Imho, this only makes
        // things more complicated and hardly seems worth the hassle, but there you have it.
        // Therefore, we copy Im[0] to Re[Nyquist]...
        real[size / 2] = imaginary[0];
        
        // ... And set Im[0] and Im[Nyquist] to 0, which they always are
        imaginary[0] = 0.0f;
        imaginary[size / 2] = 0.0f;
    }
    
    void FastFourierTransformAccelerate::doInverse(span<const float> real, span<const float> imaginary, span<float> output)
    {
        // Copy the input reals and imaginaries, so that we can change the format around to
        // the way vDSP accepts it
        vector<float> real_(real.begin(), real.end());
        vector<float> imaginary_(imaginary.begin(), imaginary.end());

        // Re[Nyquist] is supposed to be stored in Im[0] for vDSP
        imaginary_[0] = real[size / 2];

        // Do the transform
        vDSP_DFT_Execute(floatSetup.inverse, real.data(), imaginary_.data(), real_.data(), imaginary_.data());

        // Combine the even and odd output signals into one interleaved output signal
        interleave(span<const float>(real_), span<const float>(imaginary_), output);

        // For inverse DFT, the scaling is Size, so scale back by multiplying with its reciprocal
        const float factor = 1.0f / output.size();
        multiply(span<const float>(output), factor, output);
    }
    
    void FastFourierTransformAccelerate::doInverse(span<const double> real, span<const double> imaginary, span<double> output)
    {
        // Copy the input reals and imaginaries, so that we can change the format around to
        // the way vDSP accepts it
        vector<double> real_(real.begin(), real.end());
        vector<double> imaginary_(imaginary.begin(), imaginary.end());
        
        // Re[Nyquist] is supposed to be stored in Im[0] for vDSP
        imaginary_[0] = real[size / 2];
        
        // Do the transform
        vDSP_DFT_ExecuteD(doubleSetup.inverse, real.data(), imaginary_.data(), real_.data(), imaginary_.data());
        
        // Combine the even and odd output signals into one interleaved output signal
        interleave(span<const double>(real_), span<const double>(imaginary_), output);
        
        // For inverse DFT, the scaling is Size, so scale back by multiplying with its reciprocal
        const double factor = 1.0 / output.size();
        multiply(span<const double>(output), factor, output);
    }
    
    void FastFourierTransformAccelerate::doForwardComplex(span<const float> inReal, span<const float> inImaginary, span<float> outReal, span<float> outImaginary)
    {
        // Do the transform
        vDSP_DFT_Execute(floatComplexSetup.forward, inReal.data(), inImaginary.data(), outReal.data(), outImaginary.data());
    }
    
    void FastFourierTransformAccelerate::doForwardComplex(span<const double> inReal, span<const double> inImaginary, span<double> outReal, span<double> outImaginary)
    {
        // Do the transform
        vDSP_DFT_ExecuteD(doubleComplexSetup.forward, inReal.data(), inImaginary.data(), outReal.data(), outImaginary.data());
    }
    
    void FastFourierTransformAccelerate::doInverseComplex(span<const float> inReal, span<const float> inImaginary, span<float> outReal, span<float> outImaginary)
    {
        // Do the transform
        vDSP_DFT_Execute(floatComplexSetup.inverse, inReal.data(), inImaginary.data(), outReal.data(), outImaginary.data());
        
        // For inverse DFT, the scaling is Size, so scale back by multiplying with its reciprocal
        const float factor = 1.0f / outReal.size();
        multiply(span<const float>(outReal), factor, outReal);
        multiply(span<const float>(outImaginary), factor, outImaginary);
    }
    
    void FastFourierTransformAccelerate::doInverseComplex(span<const double> inReal, span<const double> inImaginary, span<double> outReal, span<double> outImaginary)
    {
        // Do the transform
        vDSP_DFT_ExecuteD(doubleComplexSetup.inverse, inReal.data(), inImaginary.data(), outReal.data(), outImaginary.data());
        
        // For inverse DFT, the scaling is Size, so scale back by multiplying with its reciprocal
        const double factor = 1.0 / outReal.size();
        multiply(span<const double>(outReal), factor, outReal);
        multiply(span<const double>(outImaginary), factor, outImaginary);
    }
}
