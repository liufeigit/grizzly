//
//  FastFourierTransform.hpp
//  bear-dsp
//
//  Created by Stijn on 10/06/2016.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef FAST_FOURIER_TRANSFORM_HPP
#define FAST_FOURIER_TRANSFORM_HPP

#include <complex>
#include <vector>

#include <dsperados/math/interleave.hpp>

#ifdef __APPLE__
#define __FFT_ACCELERATE__
#include <Accelerate/Accelerate.h>
#endif

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
    
#ifdef __APPLE__
    //! Fourier transform based on Accelerate
    class FastFourierTransformAccelerate : public FastFourierTransformBase
    {
    public:
        FastFourierTransformAccelerate(std::size_t size);
        
    private:
        void doForward(const float* input, float* real, float* imaginary) override final;
        void doForward(const double* input, double* real, double* imaginary) override final;
        
        void doInverse(const float* real, const float* imaginary, float* output) override final;
        void doInverse(const double* real, const double* imaginary, double* output) override final;
        
        void doForwardComplex(const std::vector<float>& inReal, const std::vector<float>& inImaginary, std::vector<float>& outReal, std::vector<float>& outImaginary) override final;
        void doForwardComplex(const std::vector<double>& inReal, const std::vector<double>& inImaginary, std::vector<double>& outReal, std::vector<double>& outImaginary) override final;
        
        void doInverseComplex(const std::vector<float>& inReal, const std::vector<float>& inImaginary, std::vector<float>& outReal, std::vector<float>& outImaginary) override final;
        void doInverseComplex(const std::vector<double>& inReal, const std::vector<double>& inImaginary, std::vector<double>& outReal, std::vector<double>& outImaginary) override final;
        
    private:
        std::vector<float> evenFloat;
        std::vector<float> oddFloat;
        
        std::vector<double> evenDouble;
        std::vector<double> oddDouble;
        
        struct SetupFloat
        {
            ~SetupFloat()
            {
                if (forward)
                    vDSP_DFT_DestroySetup(forward);
                
                if (inverse)
                    vDSP_DFT_DestroySetup(inverse);
            }
            
            vDSP_DFT_Setup forward = nullptr;
            vDSP_DFT_Setup inverse = nullptr;
        } floatSetup, floatComplexSetup;
        
        struct SetupDouble
        {
            ~SetupDouble()
            {
                if (forward)
                    vDSP_DFT_DestroySetupD(forward);
                
                if (inverse)
                    vDSP_DFT_DestroySetupD(inverse);
            }
            
            vDSP_DFT_SetupD forward = nullptr;
            vDSP_DFT_SetupD inverse = nullptr;
        } doubleSetup, doubleComplexSetup;
    };
#endif
    
    //! Fourier transform based on Ooura's FFT code
    class FastFourierTransformOoura : public FastFourierTransformBase
    {
    public:
        FastFourierTransformOoura(std::size_t size);
        
    private:
        void doForward(const float* input, float* real, float* imaginary) override final;
        void doForward(const double* input, double* real, double* imaginary) override final;
        
        void doInverse(const float* real, const float* imaginary, float* output) override final;
        void doInverse(const double* real, const double* imaginary, double* output) override final;
        
        void doForwardComplex(const std::vector<float>& inReal, const std::vector<float>& inImaginary, std::vector<float>& outReal, std::vector<float>& outImaginary) override final;
        void doForwardComplex(const std::vector<double>& inReal, const std::vector<double>& inImaginary, std::vector<double>& outReal, std::vector<double>& outImaginary) override final;
        
        void doInverseComplex(const std::vector<float>& inReal, const std::vector<float>& inImaginary, std::vector<float>& outReal, std::vector<float>& outImaginary) override final;
        void doInverseComplex(const std::vector<double>& inReal, const std::vector<double>& inImaginary, std::vector<double>& outReal, std::vector<double>& outImaginary) override final;
        
    private:
        std::vector<double> data;
        std::vector<int> ip;
        std::vector<double> w;
        
        std::vector<double> dataComplex;
    };
    
#ifdef __FFT_ACCELERATE__
    using FastFourierTransform = FastFourierTransformAccelerate;
#else
    using FastFourierTransform = FastFourierTransformOoura;
#endif
}

#endif
