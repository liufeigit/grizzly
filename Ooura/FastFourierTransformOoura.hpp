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
        
    private:
        void doForward(const float* input, float* real, float* imaginary) override final;
        void doForward(const double* input, double* real, double* imaginary) override final;
        
        void doInverse(const float* real, const float* imaginary, float* output) override final;
        void doInverse(const double* real, const double* imaginary, double* output) override final;
        
        void doForwardComplex(const float* inReal, const float* inImaginary, float* outReal, float* outImaginary) override final;
        void doForwardComplex(const double* inReal, const double* inImaginary, double* outReal, double* outImaginary) override final;
        
        void doInverseComplex(const float* inReal, const float* inImaginary, float* outReal, float* outImaginary) override final;
        void doInverseComplex(const double* inReal, const double* inImaginary, double* outReal, double* outImaginary) override final;
        
    private:
        std::vector<double> data;
        std::vector<int> ip;
        std::vector<double> w;
        
        std::vector<double> dataComplex;
    };
}

#endif /* GRIZZLY_FAST_FOURIER_TRANSFORM_OOURA_HPP */
