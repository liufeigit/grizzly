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
#include <gsl/span>
#include <vector>

#ifdef __APPLE__
#include <Accelerate/Accelerate.h>
#endif

namespace bear::dsp
{
    //! Base class for Fourier transforms
    class FastFourierTransformBase
    {
    public:
        //! Construct by passing the size
        FastFourierTransformBase(std::size_t size);
        virtual ~FastFourierTransformBase() = default;
        
        // Real
        
        std::vector<std::complex<float>> forward(gsl::span<const float> input);
        std::vector<std::complex<double>> forward(gsl::span<const double> input);
        
        void forward(gsl::span<const float> input, gsl::span<std::complex<float>> output);
        void forward(gsl::span<const double> input, gsl::span<std::complex<double>> output);
        
        void forward(gsl::span<const float> input, gsl::span<float> real, gsl::span<float> imaginary);
        void forward(gsl::span<const double> input, gsl::span<double> real, gsl::span<double> imaginary);
        
        std::vector<float> inverse(gsl::span<const std::complex<float>> input);
        std::vector<double> inverse(gsl::span<const std::complex<double>> input);
        
        void inverse(gsl::span<const std::complex<float>> input, gsl::span<float> output);
        void inverse(gsl::span<const std::complex<double>> input, gsl::span<double> output);
        
        void inverse(gsl::span<const float> real, gsl::span<const float> imaginary, gsl::span<float> output);
        void inverse(gsl::span<const double> real, gsl::span<const double> imaginary, gsl::span<double> output);
        
        // Complex
        
        std::vector<std::complex<float>> forwardComplex(gsl::span<const std::complex<float>> input);
        std::vector<std::complex<double>> forwardComplex(gsl::span<const std::complex<double>> input);
        
        void forwardComplex(gsl::span<const std::complex<float>> input, gsl::span<std::complex<float>> output);
        void forwardComplex(gsl::span<const std::complex<double>> input, gsl::span<std::complex<double>> output);
        
        void forwardComplex(gsl::span<const float> inReal, gsl::span<const float> inImaginary, gsl::span<float> outReal, gsl::span<float> outImaginary);
        void forwardComplex(gsl::span<const double> inReal, gsl::span<const double> inImaginary, gsl::span<double> outReal, gsl::span<double> outImaginary);
        
        std::vector<std::complex<float>> inverseComplex(gsl::span<const std::complex<float>> input);
        std::vector<std::complex<double>> inverseComplex(gsl::span<const std::complex<double>> input);
        
        void inverseComplex(gsl::span<const std::complex<float>> input, gsl::span<std::complex<float>> output);
        void inverseComplex(gsl::span<const std::complex<double>> input, gsl::span<std::complex<double>> output);
        
        void inverseComplex(gsl::span<const float> inReal, gsl::span<const float> inImaginary, gsl::span<float> outReal, gsl::span<float> outImaginary);
        void inverseComplex(gsl::span<const double> inReal, gsl::span<const double> inImaginary, gsl::span<double> outReal, gsl::span<double> outImaginary);
        
        auto getSize() const { return size; }
        
    protected:
        //! The frame size
        std::size_t size = 0;
        
    private:
        virtual void doForward(gsl::span<const float> input, gsl::span<float> real, gsl::span<float> imaginary) = 0;
        virtual void doForward(gsl::span<const double> input, gsl::span<double> real, gsl::span<double> imaginary) = 0;
        
        virtual void doInverse(gsl::span<const float> real, gsl::span<const float> imaginary, gsl::span<float> output) = 0;
        virtual void doInverse(gsl::span<const double> real, gsl::span<const double> imaginary, gsl::span<double> output) = 0;
        
        virtual void doForwardComplex(gsl::span<const float> inReal, gsl::span<const float> inImaginary, gsl::span<float> outReal, gsl::span<float> outImaginary) = 0;
        virtual void doForwardComplex(gsl::span<const double> inReal, gsl::span<const double> inImaginary, gsl::span<double> outReal, gsl::span<double> outImaginary) = 0;
        
        virtual void doInverseComplex(gsl::span<const float> inReal, gsl::span<const float> inImaginary, gsl::span<float> outReal, gsl::span<float> outImaginary) = 0;
        virtual void doInverseComplex(gsl::span<const double> inReal, gsl::span<const double> inImaginary, gsl::span<double> outReal, gsl::span<double> outImaginary) = 0;
    };
    
#ifdef __APPLE__
    //! Fourier transform based on Accelerate
    class FastFourierTransformAccelerate : public FastFourierTransformBase
    {
    public:
        FastFourierTransformAccelerate(std::size_t size);
        
    private:
        void doForward(gsl::span<const float> input, gsl::span<float> real, gsl::span<float> imaginary) override final;
        void doForward(gsl::span<const double> input, gsl::span<double> real, gsl::span<double> imaginary) override final;
        
        void doInverse(gsl::span<const float> real, gsl::span<const float> imaginary, gsl::span<float> output) override final;
        void doInverse(gsl::span<const double> real, gsl::span<const double> imaginary, gsl::span<double> output) override final;
        
        void doForwardComplex(gsl::span<const float> inReal, gsl::span<const float> inImaginary, gsl::span<float> outReal, gsl::span<float> outImaginary) override final;
        void doForwardComplex(gsl::span<const double> inReal, gsl::span<const double> inImaginary, gsl::span<double> outReal, gsl::span<double> outImaginary) override final;
        
        void doInverseComplex(gsl::span<const float> inReal, gsl::span<const float> inImaginary, gsl::span<float> outReal, gsl::span<float> outImaginary) override final;
        void doInverseComplex(gsl::span<const double> inReal, gsl::span<const double> inImaginary, gsl::span<double> outReal, gsl::span<double> outImaginary) override final;
        
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
        void doForward(gsl::span<const float> input, gsl::span<float> real, gsl::span<float> imaginary) override final;
        void doForward(gsl::span<const double> input, gsl::span<double> real, gsl::span<double> imaginary) override final;
        
        void doInverse(gsl::span<const float> real, gsl::span<const float> imaginary, gsl::span<float> output) override final;
        void doInverse(gsl::span<const double> real, gsl::span<const double> imaginary, gsl::span<double> output) override final;
        
        void doForwardComplex(gsl::span<const float> inReal, gsl::span<const float> inImaginary, gsl::span<float> outReal, gsl::span<float> outImaginary) override final;
        void doForwardComplex(gsl::span<const double> inReal, gsl::span<const double> inImaginary, gsl::span<double> outReal, gsl::span<double> outImaginary) override final;
        
        void doInverseComplex(gsl::span<const float> inReal, gsl::span<const float> inImaginary, gsl::span<float> outReal, gsl::span<float> outImaginary) override final;
        void doInverseComplex(gsl::span<const double> inReal, gsl::span<const double> inImaginary, gsl::span<double> outReal, gsl::span<double> outImaginary) override final;
        
    private:
        std::vector<double> data;
        std::vector<int> ip;
        std::vector<double> w;
        
        std::vector<double> dataComplex;
    };
    
#ifdef __APPLE__
    using FastFourierTransform = FastFourierTransformAccelerate;
#else
    using FastFourierTransform = FastFourierTransformOoura;
#endif
}

#endif
