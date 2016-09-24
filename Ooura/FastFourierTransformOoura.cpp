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

    void FastFourierTransformOoura::doForward(span<const float> input, span<float> real, span<float> imaginary)
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
    
    void FastFourierTransformOoura::doForward(span<const double> input, span<double> real, span<double> imaginary)
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
    
    void FastFourierTransformOoura::doInverse(span<const float> real, span<const float> imaginary, span<float> output)
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
        multiply(span<const float>(output), factor, output);
    }
    
    void FastFourierTransformOoura::doInverse(span<const double> real, span<const double> imaginary, span<double> output)
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
        multiply(span<const double>(output), factor, output);
    }
    
    void FastFourierTransformOoura::doForwardComplex(span<const float> inReal, span<const float> inImaginary, span<float> outReal, span<float> outImaginary)
    {
        interleave(inReal, inImaginary, span<double>(dataComplex));
        
        cdft(static_cast<int>(size * 2), 1, dataComplex.data(), ip.data(), w.data());

        deinterleave(span<const double>(dataComplex), outReal, outImaginary);
    }
    
    void FastFourierTransformOoura::doForwardComplex(span<const double> inReal, span<const double> inImaginary, span<double> outReal, span<double> outImaginary)
    {
        interleave(inReal, inImaginary, span<double>(dataComplex));
        
        cdft(static_cast<int>(size * 2), 1, dataComplex.data(), ip.data(), w.data());
        
        deinterleave(span<const double>(dataComplex), outReal, outImaginary);
    }
    
    void FastFourierTransformOoura::doInverseComplex(span<const float> inReal, span<const float> inImaginary, span<float> outReal, span<float> outImaginary)
    {
        interleave(inReal, inImaginary, span<double>(dataComplex));
        
        cdft(static_cast<int>(size * 2), -1, dataComplex.data(), ip.data(), w.data());
        
        float factor = 1.0 / size;
        multiply(span<const double>(dataComplex), factor, span<double>(dataComplex));
        
        deinterleave(span<const double>(dataComplex), outReal, outImaginary);
    }
    
    void FastFourierTransformOoura::doInverseComplex(span<const double> inReal, span<const double> inImaginary, span<double> outReal, span<double> outImaginary)
    {
        interleave(inReal, inImaginary, span<double>(dataComplex));
        
        cdft(static_cast<int>(size * 2), -1, dataComplex.data(), ip.data(), w.data());
        
        double factor = 1.0 / size;
        multiply(span<const double>(dataComplex), factor, span<double>(dataComplex));
        
        deinterleave(span<const double>(dataComplex), outReal, outImaginary);
    }
}