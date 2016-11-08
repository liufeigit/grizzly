//
//  FastFourierTransformOoura.cpp
//  bear-dsp
//
//  Created by Stijn on 09/06/2016.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#include "../../Core/Parallel.hpp"
#include "../FastFourierTransform.hpp"
#include "fftsg.h"

using namespace gsl;
using namespace std;

namespace bear::dsp
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

    void FastFourierTransformOoura::doForward(const vector<float>& input, vector<float>& real, vector<float>& imaginary)
    {
        data.assign(input.begin(), input.end());
        
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
    
    void FastFourierTransformOoura::doForward(const vector<double>& input, vector<double>& real, vector<double>& imaginary)
    {
        data.assign(input.begin(), input.end());
        
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
    
    void FastFourierTransformOoura::doInverse(const vector<float>& real, const vector<float>& imaginary, vector<float>& output)
    {
        for (auto i = 0; i < size / 2; ++i)
        {
            data[i * 2] = real[i];
            data[i * 2 + 1] = imaginary[i];
        }
        
        data[1] = real[size / 2];
        
        rdft(static_cast<int>(size), -1, data.data(), ip.data(), w.data());
        
        move(data.begin(), data.end(), output.begin());
        
        float factor = 2.0 / size;
        multiply(const vector<float>&(output), factor, output);
    }
    
    void FastFourierTransformOoura::doInverse(const vector<double>& real, const vector<double>& imaginary, vector<double>& output)
    {
        for (auto i = 0; i < size / 2; ++i)
        {
            data[i * 2] = real[i];
            data[i * 2 + 1] = imaginary[i];
        }
        
        data[1] = real[size / 2];
        
        rdft(static_cast<int>(size), -1, data.data(), ip.data(), w.data());
        
        move(data.begin(), data.end(), output.begin());
        
        float factor = 2.0 / size;
        multiply(const vector<double>&(output), factor, output);
    }
    
    void FastFourierTransformOoura::doForwardComplex(const vector<float>& inReal, const vector<float>& inImaginary, vector<float>& outReal, vector<float>& outImaginary)
    {
        interleave(inReal, inImaginary, vector<double>&(dataComplex));
        
        cdft(static_cast<int>(size * 2), 1, dataComplex.data(), ip.data(), w.data());

        deinterleave(const vector<double>&(dataComplex), outReal, outImaginary);
    }
    
    void FastFourierTransformOoura::doForwardComplex(const vector<double>& inReal, const vector<double>& inImaginary, vector<double>& outReal, vector<double>& outImaginary)
    {
        interleave(inReal, inImaginary, vector<double>&(dataComplex));
        
        cdft(static_cast<int>(size * 2), 1, dataComplex.data(), ip.data(), w.data());
        
        deinterleave(const vector<double>&(dataComplex), outReal, outImaginary);
    }
    
    void FastFourierTransformOoura::doInverseComplex(const vector<float>& inReal, const vector<float>& inImaginary, vector<float>& outReal, vector<float>& outImaginary)
    {
        interleave(inReal, inImaginary, vector<double>&(dataComplex));
        
        cdft(static_cast<int>(size * 2), -1, dataComplex.data(), ip.data(), w.data());
        
        float factor = 1.0 / size;
        multiply(const vector<double>&(dataComplex), factor, vector<double>&(dataComplex));
        
        deinterleave(const vector<double>&(dataComplex), outReal, outImaginary);
    }
    
    void FastFourierTransformOoura::doInverseComplex(const vector<double>& inReal, const vector<double>& inImaginary, vector<double>& outReal, vector<double>& outImaginary)
    {
        interleave(inReal, inImaginary, vector<double>&(dataComplex));
        
        cdft(static_cast<int>(size * 2), -1, dataComplex.data(), ip.data(), w.data());
        
        double factor = 1.0 / size;
        multiply(const vector<double>&(dataComplex), factor, vector<double>&(dataComplex));
        
        deinterleave(const vector<double>&(dataComplex), outReal, outImaginary);
    }
}