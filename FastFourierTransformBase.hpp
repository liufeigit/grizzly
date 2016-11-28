//
//  FastFourierTransformBase.hpp
//  bear-dsp
//
//  Created by Stijn on 10/06/2016.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef FAST_FOURIER_TRANSFORM_BASE_HPP
#define FAST_FOURIER_TRANSFORM_BASE_HPP

#include <cstddef>
#include <complex>
#include <vector>

namespace dsp
{
    //! Base class for Fourier transforms
    class FastFourierTransformBase
    {
    public:
        //! Construct by passing the size
        FastFourierTransformBase(std::size_t size);
        virtual ~FastFourierTransformBase() = default;
        
        // Real
        
        std::vector<std::complex<float>> forward(const std::vector<float>& input);
        std::vector<std::complex<double>> forward(const std::vector<double>& input);
        
        void forward(const std::vector<float>& input, std::vector<std::complex<float>>& output);
        void forward(const std::vector<double>& input, std::vector<std::complex<double>>& output);
        
        void forward(const std::vector<float>& input, std::vector<float>& real, std::vector<float>& imaginary);
        void forward(const std::vector<double>& input, std::vector<double>& real, std::vector<double>& imaginary);
        
        std::vector<float> inverse(const std::vector<std::complex<float>>& input);
        std::vector<double> inverse(const std::vector<std::complex<double>>& input);
        
        void inverse(const std::vector<std::complex<float>>& input, std::vector<float>& output);
        void inverse(const std::vector<std::complex<double>>& input, std::vector<double>& output);
        
        void inverse(const std::vector<float>& real, const std::vector<float>& imaginary, std::vector<float>& output);
        void inverse(const std::vector<double>& real, const std::vector<double>& imaginary, std::vector<double>& output);
        
        // Complex
        
        std::vector<std::complex<float>> forwardComplex(const std::vector<std::complex<float>>& input);
        std::vector<std::complex<double>> forwardComplex(const std::vector<std::complex<double>>& input);
        
        void forwardComplex(const std::vector<std::complex<float>>& input, std::vector<std::complex<float>>& output);
        void forwardComplex(const std::vector<std::complex<double>>& input, std::vector<std::complex<double>>& output);
        
        void forwardComplex(const std::vector<float>& inReal, const std::vector<float>& inImaginary, std::vector<float>& outReal, std::vector<float>& outImaginary);
        void forwardComplex(const std::vector<double>& inReal, const std::vector<double>& inImaginary, std::vector<double>& outReal, std::vector<double>& outImaginary);
        
        std::vector<std::complex<float>> inverseComplex(const std::vector<std::complex<float>>& input);
        std::vector<std::complex<double>> inverseComplex(const std::vector<std::complex<double>>& input);
        
        void inverseComplex(const std::vector<std::complex<float>>& input, std::vector<std::complex<float>>& output);
        void inverseComplex(const std::vector<std::complex<double>>& input, std::vector<std::complex<double>>& output);
        
        void inverseComplex(const std::vector<float>& inReal, const std::vector<float>& inImaginary, std::vector<float>& outReal, std::vector<float>& outImaginary);
        void inverseComplex(const std::vector<double>& inReal, const std::vector<double>& inImaginary, std::vector<double>& outReal, std::vector<double>& outImaginary);
        
        auto getSize() const { return size; }
        
    protected:
        //! The frame size
        std::size_t size = 0;
        
    private:
        virtual void doForward(const float* input, float* real, float* imaginary) = 0;
        virtual void doForward(const double* input, double* real, double* imaginary) = 0;
        
        virtual void doInverse(const float* real, const float* imaginary, float* output) = 0;
        virtual void doInverse(const double* real, const double* imaginary, double* output) = 0;
        
        virtual void doForwardComplex(const std::vector<float>& inReal, const std::vector<float>& inImaginary, std::vector<float>& outReal, std::vector<float>& outImaginary) = 0;
        virtual void doForwardComplex(const std::vector<double>& inReal, const std::vector<double>& inImaginary, std::vector<double>& outReal, std::vector<double>& outImaginary) = 0;
        
        virtual void doInverseComplex(const std::vector<float>& inReal, const std::vector<float>& inImaginary, std::vector<float>& outReal, std::vector<float>& outImaginary) = 0;
        virtual void doInverseComplex(const std::vector<double>& inReal, const std::vector<double>& inImaginary, std::vector<double>& outReal, std::vector<double>& outImaginary) = 0;
    };
}

#endif
