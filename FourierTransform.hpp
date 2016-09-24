//
//  FourierTransform.hpp
//  libbear
//
//  Created by Stijn on 04/11/2015.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef BEAR_DSP_FOURIER_TRANSFORM_HPP
#define BEAR_DSP_FOURIER_TRANSFORM_HPP

#include <complex>
#include <gsl/span.h>
#include <vector>

#include "FastFourierTransform.hpp"

namespace bear::dsp
{
    //! The real-to-complex forward Fourier transform
    void fourierTransform(gsl::span<const float> input, gsl::span<std::complex<float>> output);
    
    //! The real-to-complex forward Fourier transform
    void fourierTransform(gsl::span<const float> input, gsl::span<float> outputReal, gsl::span<float> outputImaginary);
    
    //! The complex-to-complex forward Fourier transform
    void fourierTransformComplex(gsl::span<const std::complex<float>> input, gsl::span<std::complex<float>> output);
    
    //! The complex-to-complex forward Fourier transform
    void fourierTransformComplex(gsl::span<const float> inputReal, gsl::span<const float> inputImaginary, gsl::span<float> outputReal, gsl::span<float> outputImaginary);
    
    //! The real-to-complex forward Fourier transform
    std::vector<std::complex<float>> fourierTransform(gsl::span<const float> input);
    
    //! The complex-to-complex forward Fourier transform
    std::vector<std::complex<float>> fourierTransformComplex(gsl::span<const std::complex<float>> input);
    
    //! The complex-to-real inverse Fourier transform
    void inverseFourierTransform(gsl::span<const std::complex<float>> input, gsl::span<float> output);
    
    //! The complex-to-real inverse Fourier transform
    void inverseFourierTransform(gsl::span<const float> inputReal, gsl::span<const float> inputImaginary, gsl::span<float> output);
    
    //! The complex-to-complex inverse Fourier transform
    void inverseFourierTransformComplex(gsl::span<const std::complex<float>> input, gsl::span<std::complex<float>> output);
    
    //! the complex-to-complex inverse Fourier transform
    void inverseFourierTransformComplex(gsl::span<const float> inputReal, gsl::span<const float> inputImaginary, gsl::span<float> outputReal, gsl::span<float> outputImaginary);
    
    //! The complex-to-real inverse Fourier transform
    std::vector<float> inverseFourierTransform(gsl::span<const std::complex<float>> input);
    
    //! The complex-to-complex inverse Fourier transform
    std::vector<std::complex<float>> inverseFourierTransformComplex(gsl::span<const std::complex<float>> input);
    
    //! The short-time Fourier transform
    std::vector<std::vector<std::complex<float>>> shortTimeFourierTransform(gsl::span<const float> input, FastFourierTransformBase& fourier, gsl::span<const float> window, size_t hopSize);
    
    //! The short-time Fourier transform
    std::vector<std::vector<std::complex<float>>> shortTimeFourierTransform(gsl::span<const float> input, size_t frameSize, gsl::span<const float> window, size_t hopSize);
}

#endif