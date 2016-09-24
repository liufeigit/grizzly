//
//  FastFourierTransform.cpp
//  bear-dsp
//
//  Created by Stijn on 10/06/2016.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#include "FastFourierTransform.hpp"
#include "Parallel.hpp"

using namespace gsl;
using namespace std;

namespace bear::dsp
{
    FastFourierTransformBase::FastFourierTransformBase(size_t size) :
        size(size)
    {
        
    }
    
    vector<complex<float>> FastFourierTransformBase::forward(span<const float> input)
    {
        vector<complex<float>> output(input.size() / 2 + 1);
        forward(input, output);
        return output;
    }
    
    vector<complex<double>> FastFourierTransformBase::forward(span<const double> input)
    {
        vector<complex<double>> output(input.size() / 2 + 1);
        forward(input, output);
        return output;
    }
    
    void FastFourierTransformBase::forward(span<const float> input, span<complex<float>> output)
    {
        // The deinterleaved output will be stored in here
        vector<float> real(output.size(), 0);
        vector<float> imaginary(output.size(), 0);
        
        // Do the forward transform
        forward(input, real, imaginary);
        
        interleave(span<const float>(real), span<const float>(imaginary), span<float>((float*)output.data(), output.size() * 2));
    }
    
    void FastFourierTransformBase::forward(span<const double> input, span<complex<double>> output)
    {
        // The deinterleaved output will be stored in here
        vector<double> real(output.size(), 0);
        vector<double> imaginary(output.size(), 0);
        
        // Do the forward transform
        forward(input, real, imaginary);
        
        interleave(span<const double>(real), span<const double>(imaginary), span<double>((double*)output.data(), output.size() * 2));
    }
    
    void FastFourierTransformBase::forward(span<const float> input, span<float> real, span<float> imaginary)
    {
        assert(input.size() == size);
        assert(real.size() == size / 2 + 1);
        assert(imaginary.size() == size / 2 + 1);
        
        doForward(input, real, imaginary);
    }
    
    void FastFourierTransformBase::forward(span<const double> input, span<double> real, span<double> imaginary)
    {
        assert(input.size() == size);
        assert(real.size() == size / 2 + 1);
        assert(imaginary.size() == size / 2 + 1);
        
        doForward(input, real, imaginary);
    }
    
    vector<float> FastFourierTransformBase::inverse(span<const complex<float>> input)
    {
        vector<float> output((input.size() - 1) * 2);
        inverse(input, output);
        return output;
    }
    
    vector<double> FastFourierTransformBase::inverse(span<const complex<double>> input)
    {
        vector<double> output((input.size() - 1) * 2);
        inverse(input, output);
        return output;
    }
    
    void FastFourierTransformBase::inverse(span<const complex<float>> input, span<float> output)
    {
        // The deinterleaved input is stored in here
        vector<float> real(input.size());
        vector<float> imaginary(input.size());
        
        // Deinterleave
        deinterleave(span<const float>((float*)input.data(), input.size() * 2), span<float>(real), span<float>(imaginary));
        
        // Do the inverse transform
        inverse(real, imaginary, output);
    }
    
    void FastFourierTransformBase::inverse(span<const complex<double>> input, span<double> output)
    {
        // The deinterleaved input is stored in here
        vector<double> real(input.size());
        vector<double> imaginary(input.size());
        
        // Deinterleave
        deinterleave(span<const double>((double*)input.data(), input.size() * 2), span<double>(real), span<double>(imaginary));
        
        // Do the inverse transform
        inverse(real, imaginary, output);
    }
    
    void FastFourierTransformBase::inverse(span<const float> real, span<const float> imaginary, span<float> output)
    {
        assert(real.size() == size / 2 + 1);
        assert(imaginary.size() == size / 2 + 1);
        assert(output.size() == size);
        
        doInverse(real, imaginary, output);
    }
    
    void FastFourierTransformBase::inverse(span<const double> real, span<const double> imaginary, span<double> output)
    {
        assert(real.size() == size / 2 + 1);
        assert(imaginary.size() == size / 2 + 1);
        assert(output.size() == size);
        
        doInverse(real, imaginary, output);
    }
    
    vector<complex<float>> FastFourierTransformBase::forwardComplex(span<const complex<float>> input)
    {
        vector<complex<float>> output(input.size());
        
        forwardComplex(input, output);
        
        return output;
    }
    
    vector<complex<double>> FastFourierTransformBase::forwardComplex(span<const complex<double>> input)
    {
        vector<complex<double>> output(input.size());
        
        forwardComplex(input, output);
        
        return output;
    }
    
    void FastFourierTransformBase::forwardComplex(span<const complex<float>> input, span<complex<float>> output)
    {
        // The deinterleaved input is stored in here
        vector<float> inReal(input.size());
        vector<float> inImaginary(input.size());
        
        // Deinterleave
        deinterleave(span<const float>((float*)input.data(), input.size() * 2), span<float>(inReal), span<float>(inImaginary));
        
        // The deinterleaved output will be stored in here
        vector<float> outReal(output.size(), 0);
        vector<float> outImaginary(output.size(), 0);
        
        // Do the forward transform
        forwardComplex(inReal, inImaginary, outReal, outImaginary);
        
        interleave(span<const float>(outReal), span<const float>(outImaginary), span<float>((float*)output.data(), output.size() * 2));
    }
    
    void FastFourierTransformBase::forwardComplex(span<const complex<double>> input, span<complex<double>> output)
    {
        // The deinterleaved input is stored in here
        vector<double> inReal(input.size());
        vector<double> inImaginary(input.size());
        
        // Deinterleave
        deinterleave(span<const double>((double*)input.data(), input.size() * 2), span<double>(inReal), span<double>(inImaginary));
        
        // The deinterleaved output will be stored in here
        vector<double> outReal(output.size(), 0);
        vector<double> outImaginary(output.size(), 0);
        
        // Do the forward transform
        forwardComplex(inReal, inImaginary, outReal, outImaginary);
        
        interleave(span<const double>(outReal), span<const double>(outImaginary), span<double>((double*)output.data(), output.size() * 2));
    }
    
    void FastFourierTransformBase::forwardComplex(span<const float> inReal, span<const float> inImaginary, span<float> outReal, span<float> outImaginary)
    {
        assert(inReal.size() == size);
        assert(inImaginary.size() == size);
        assert(outReal.size() == size);
        assert(outImaginary.size() == size);
        
        doForwardComplex(inReal, inImaginary, outReal, outImaginary);
    }
    
    void FastFourierTransformBase::forwardComplex(span<const double> inReal, span<const double> inImaginary, span<double> outReal, span<double> outImaginary)
    {
        assert(inReal.size() == size);
        assert(inImaginary.size() == size);
        assert(outReal.size() == size);
        assert(outImaginary.size() == size);
        
        doForwardComplex(inReal, inImaginary, outReal, outImaginary);
    }
    
    vector<complex<float>> FastFourierTransformBase::inverseComplex(span<const complex<float>> input)
    {
        vector<complex<float>> output(input.size());
        
        inverseComplex(input, output);
        
        return output;
    }
    
    vector<complex<double>> FastFourierTransformBase::inverseComplex(span<const complex<double>> input)
    {
        vector<complex<double>> output(input.size());
        
        inverseComplex(input, output);
        
        return output;
    }
    
    void FastFourierTransformBase::inverseComplex(span<const complex<float>> input, span<complex<float>> output)
    {
        // The deinterleaved input is stored in here
        vector<float> inputReal(input.size());
        vector<float> inputImaginary(input.size());
        
        // Deinterleave
        deinterleave(span<const float>((float*)input.data(), input.size() * 2), span<float>(inputReal), span<float>(inputImaginary));
        
        vector<float> outputReal(output.size());
        vector<float> outputImaginary(output.size());
        
        // Do the inverse transform
        inverseComplex(inputReal, inputImaginary, outputReal, outputImaginary);
        
        interleave(span<const float>(outputReal), span<const float>(outputImaginary), span<float>((float*)output.data(), output.size() * 2));
    }
    
    void FastFourierTransformBase::inverseComplex(span<const complex<double>> input, span<complex<double>> output)
    {
        // The deinterleaved input is stored in here
        vector<float> inputReal(input.size());
        vector<float> inputImaginary(input.size());
        
        // Deinterleave
        deinterleave(span<const float>((float*)input.data(), input.size() * 2), span<float>(inputReal), span<float>(inputImaginary));
        
        vector<float> outputReal(output.size());
        vector<float> outputImaginary(output.size());
        
        // Do the inverse transform
        inverseComplex(inputReal, inputImaginary, outputReal, outputImaginary);
        
        interleave(span<const float>(outputReal), span<const float>(outputImaginary), span<float>((float*)output.data(), output.size() * 2));
    }
    
    void FastFourierTransformBase::inverseComplex(span<const float> inReal, span<const float> inImaginary, span<float> outReal, span<float> outImaginary)
    {
        assert(inReal.size() == size);
        assert(inImaginary.size() == size);
        assert(outReal.size() == size);
        assert(outImaginary.size() == size);
        
        doInverseComplex(inReal, inImaginary, outReal, outImaginary);
    }
    
    void FastFourierTransformBase::inverseComplex(span<const double> inReal, span<const double> inImaginary, span<double> outReal, span<double> outImaginary)
    {
        assert(inReal.size() == size);
        assert(inImaginary.size() == size);
        assert(outReal.size() == size);
        assert(outImaginary.size() == size);
        
        doInverseComplex(inReal, inImaginary, outReal, outImaginary);
    }
}
