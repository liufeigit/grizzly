/*
 
 This file is a part of Grizzly, a modern C++ library for digital signal
 processing. See https://github.com/dsperados/grizzly for more information.
 
 Copyright (C) 2016 Dsperados <info@dsperados.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 --------------------------------------------------------------------
 
 If you would like to use Grizzly for commercial or closed-source
 purposes, please contact us for a commercial license.
 
 */

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
