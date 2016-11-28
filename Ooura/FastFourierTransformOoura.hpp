//
//  FastFourierTransformOoura.hpp
//  grizzly
//
//  Created by Stijn Frishert on 11/28/16.
//
//

#ifndef GRIZZLY_FAST_FOURIER_TRANSFORM_OOURA_HPP
#define GRIZZLY_FAST_FOURIER_TRANSFORM_OOURA_HPP

#include <cstddef>
#include <vector>

#include "../FastFourierTransformBase.hpp"

namespace dsp
{
    //! Fourier transform based on Ooura's FFT code
    class FastFourierTransformOoura : public FastFourierTransformBase
    {
    public:
        FastFourierTransformOoura(std::size_t size);
    
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
        std::vector<double> data;
        std::vector<int> ip;
        std::vector<double> w;
        
        std::vector<double> dataComplex;
    };
}

#endif /* GRIZZLY_FAST_FOURIER_TRANSFORM_OOURA_HPP */
