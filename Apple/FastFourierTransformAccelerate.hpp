//
//  FastFourierTransformAccelerate.hpp
//  grizzly
//
//  Created by Stijn Frishert on 11/28/16.
//
//

#ifndef GRIZZLY_FAST_FOURIER_TRANSFORM_ACCELERATE_HPP
#define GRIZZLY_FAST_FOURIER_TRANSFORM_ACCELERATE_HPP

#include <Accelerate/Accelerate.h>
#include <cstddef>
#include <vector>

#include "../FastFourierTransformBase.hpp"

namespace dsp
{
    //! Fourier transform based on Accelerate
    class FastFourierTransformAccelerate : public FastFourierTransformBase
    {
    public:
        FastFourierTransformAccelerate(std::size_t size);
        
        using FastFourierTransformBase::forward;
        using FastFourierTransformBase::inverse;
        using FastFourierTransformBase::forwardComplex;
        using FastFourierTransformBase::inverseComplex;
        
        void forward(const float* input, float* real, float* imaginary) override final;
        void forward(const double* input, double* real, double* imaginary) override final;
        
        void inverse(const float* real, const float* imaginary, float* output) override final;
        void inverse(const double* real, const double* imaginary, double* output) override final;
        
        void forwardComplex(const float* inReal, const float* inImaginary, float* outReal, float* outImaginary) override final;
        void forwardComplex(const double* inReal, const double* inImaginary, double* outReal, double* outImaginary) override final;
        
        void inverseComplex(const float* inReal, const float* inImaginary, float* outReal, float* outImaginary) override final;
        void inverseComplex(const double* inReal, const double* inImaginary, double* outReal, double* outImaginary) override final;
        
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
}

#endif /* GRIZZLY_FAST_FOURIER_TRANSFORM_ACCELERATE_HPP */
