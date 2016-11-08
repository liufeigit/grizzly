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
    
    vector<complex<float>> FastFourierTransformBase::forward(const vector<float>& input)
    {
        vector<complex<float>> output(input.size() / 2 + 1);
        forward(input, output);
        return output;
    }
    
    vector<complex<double>> FastFourierTransformBase::forward(const vector<double>& input)
    {
        vector<complex<double>> output(input.size() / 2 + 1);
        forward(input, output);
        return output;
    }
    
    void FastFourierTransformBase::forward(const vector<float>& input, vector<complex<float>>& output)
    {
        // The deinterleaved output will be stored in here
        vector<float> real(output.size(), 0);
        vector<float> imaginary(output.size(), 0);
        
        // Do the forward transform
        forward(input, real, imaginary);
        
        interleave(real, imaginary, vector<float>&((float*)output.data(), output.size() * 2));
    }
    
    void FastFourierTransformBase::forward(const vector<double>& input, vector<complex<double>>& output)
    {
        // The deinterleaved output will be stored in here
        vector<double> real(output.size(), 0);
        vector<double> imaginary(output.size(), 0);
        
        // Do the forward transform
        forward(input, real, imaginary);
        
        interleave(real, imaginary, vector<double>&((double*)output.data(), output.size() * 2));
    }
    
    void FastFourierTransformBase::forward(const vector<float>& input, vector<float>& real, vector<float>& imaginary)
    {
        assert(input.size() == size);
        assert(real.size() == size / 2 + 1);
        assert(imaginary.size() == size / 2 + 1);
        
        doForward(input, real, imaginary);
    }
    
    void FastFourierTransformBase::forward(const vector<double>& input, vector<double>& real, vector<double>& imaginary)
    {
        assert(input.size() == size);
        assert(real.size() == size / 2 + 1);
        assert(imaginary.size() == size / 2 + 1);
        
        doForward(input, real, imaginary);
    }
    
    vector<float> FastFourierTransformBase::inverse(const vector<complex<float>>& input)
    {
        vector<float> output((input.size() - 1) * 2);
        inverse(input, output);
        return output;
    }
    
    vector<double> FastFourierTransformBase::inverse(const vector<complex<double>>& input)
    {
        vector<double> output((input.size() - 1) * 2);
        inverse(input, output);
        return output;
    }
    
    void FastFourierTransformBase::inverse(const vector<complex<float>>& input, vector<float>& output)
    {
        // The deinterleaved input is stored in here
        vector<float> real(input.size());
        vector<float> imaginary(input.size());
        
        // Deinterleave
        deinterleave(const vector<float>&((float*)input.data(), input.size() * 2), real, imaginary);
        
        // Do the inverse transform
        inverse(real, imaginary, output);
    }
    
    void FastFourierTransformBase::inverse(const vector<complex<double>>& input, vector<double>& output)
    {
        // The deinterleaved input is stored in here
        vector<double> real(input.size());
        vector<double> imaginary(input.size());
        
        // Deinterleave
        deinterleave(const vector<double>&((double*)input.data(), input.size() * 2), real, imaginary);
        
        // Do the inverse transform
        inverse(real, imaginary, output);
    }
    
    void FastFourierTransformBase::inverse(const vector<float>& real, const vector<float>& imaginary, vector<float>& output)
    {
        assert(real.size() == size / 2 + 1);
        assert(imaginary.size() == size / 2 + 1);
        assert(output.size() == size);
        
        doInverse(real, imaginary, output);
    }
    
    void FastFourierTransformBase::inverse(const vector<double>& real, const vector<double>& imaginary, vector<double>& output)
    {
        assert(real.size() == size / 2 + 1);
        assert(imaginary.size() == size / 2 + 1);
        assert(output.size() == size);
        
        doInverse(real, imaginary, output);
    }
    
    vector<complex<float>> FastFourierTransformBase::forwardComplex(const vector<complex<float>>& input)
    {
        vector<complex<float>> output(input.size());
        
        forwardComplex(input, output);
        
        return output;
    }
    
    vector<complex<double>> FastFourierTransformBase::forwardComplex(const vector<complex<double>>& input)
    {
        vector<complex<double>> output(input.size());
        
        forwardComplex(input, output);
        
        return output;
    }
    
    void FastFourierTransformBase::forwardComplex(const vector<complex<float>>& input, vector<complex<float>>& output)
    {
        // The deinterleaved input is stored in here
        vector<float> inReal(input.size());
        vector<float> inImaginary(input.size());
        
        // Deinterleave
        deinterleave(const vector<float>&((float*)input.data(), input.size() * 2), inReal, inImaginary);
        
        // The deinterleaved output will be stored in here
        vector<float> outReal(output.size(), 0);
        vector<float> outImaginary(output.size(), 0);
        
        // Do the forward transform
        forwardComplex(inReal, inImaginary, outReal, outImaginary);
        
        interleave(outReal, outImaginary, vector<float>&((float*)output.data(), output.size() * 2));
    }
    
    void FastFourierTransformBase::forwardComplex(const vector<complex<double>>& input, vector<complex<double>>& output)
    {
        // The deinterleaved input is stored in here
        vector<double> inReal(input.size());
        vector<double> inImaginary(input.size());
        
        // Deinterleave
        deinterleave(const vector<double>&((double*)input.data(), input.size() * 2), inReal, inImaginary);
        
        // The deinterleaved output will be stored in here
        vector<double> outReal(output.size(), 0);
        vector<double> outImaginary(output.size(), 0);
        
        // Do the forward transform
        forwardComplex(inReal, inImaginary, outReal, outImaginary);
        
        interleave(outReal, outImaginary, vector<double>&((double*)output.data(), output.size() * 2));
    }
    
    void FastFourierTransformBase::forwardComplex(const vector<float>& inReal, const vector<float>& inImaginary, vector<float>& outReal, vector<float>& outImaginary)
    {
        assert(inReal.size() == size);
        assert(inImaginary.size() == size);
        assert(outReal.size() == size);
        assert(outImaginary.size() == size);
        
        doForwardComplex(inReal, inImaginary, outReal, outImaginary);
    }
    
    void FastFourierTransformBase::forwardComplex(const vector<double>& inReal, const vector<double>& inImaginary, vector<double>& outReal, vector<double>& outImaginary)
    {
        assert(inReal.size() == size);
        assert(inImaginary.size() == size);
        assert(outReal.size() == size);
        assert(outImaginary.size() == size);
        
        doForwardComplex(inReal, inImaginary, outReal, outImaginary);
    }
    
    vector<complex<float>> FastFourierTransformBase::inverseComplex(const vector<complex<float>>& input)
    {
        vector<complex<float>> output(input.size());
        
        inverseComplex(input, output);
        
        return output;
    }
    
    vector<complex<double>> FastFourierTransformBase::inverseComplex(const vector<complex<double>>& input)
    {
        vector<complex<double>> output(input.size());
        
        inverseComplex(input, output);
        
        return output;
    }
    
    void FastFourierTransformBase::inverseComplex(const vector<complex<float>>& input, vector<complex<float>>& output)
    {
        // The deinterleaved input is stored in here
        vector<float> inputReal(input.size());
        vector<float> inputImaginary(input.size());
        
        // Deinterleave
        deinterleave(const vector<float>&((float*)input.data(), input.size() * 2), inputReal, inputImaginary);
        
        vector<float> outputReal(output.size());
        vector<float> outputImaginary(output.size());
        
        // Do the inverse transform
        inverseComplex(inputReal, inputImaginary, outputReal, outputImaginary);
        
        interleave(outputReal, outputImaginary, vector<float>&((float*)output.data(), output.size() * 2));
    }
    
    void FastFourierTransformBase::inverseComplex(const vector<complex<double>>& input, vector<complex<double>>& output)
    {
        // The deinterleaved input is stored in here
        vector<float> inputReal(input.size());
        vector<float> inputImaginary(input.size());
        
        // Deinterleave
        deinterleave(const vector<float>&((float*)input.data(), input.size() * 2), inputReal, inputImaginary);
        
        vector<float> outputReal(output.size());
        vector<float> outputImaginary(output.size());
        
        // Do the inverse transform
        inverseComplex(inputReal, inputImaginary, outputReal, outputImaginary);
        
        interleave(outputReal, outputImaginary, vector<float>&((float*)output.data(), output.size() * 2));
    }
    
    void FastFourierTransformBase::inverseComplex(const vector<float>& inReal, const vector<float>& inImaginary, vector<float>& outReal, vector<float>& outImaginary)
    {
        assert(inReal.size() == size);
        assert(inImaginary.size() == size);
        assert(outReal.size() == size);
        assert(outImaginary.size() == size);
        
        doInverseComplex(inReal, inImaginary, outReal, outImaginary);
    }
    
    void FastFourierTransformBase::inverseComplex(const vector<double>& inReal, const vector<double>& inImaginary, vector<double>& outReal, vector<double>& outImaginary)
    {
        assert(inReal.size() == size);
        assert(inImaginary.size() == size);
        assert(outReal.size() == size);
        assert(outImaginary.size() == size);
        
        doInverseComplex(inReal, inImaginary, outReal, outImaginary);
    }
}
