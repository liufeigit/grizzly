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
#include <vector>

#include "FastFourierTransform.hpp"
#include "Spectrum.hpp"

namespace bear::dsp
{
    //! The real-to-complex forward Fourier transform
    void fourierTransform(const std::vector<float>& input, std::vector<std::complex<float>>& output);
    
    //! The real-to-complex forward Fourier transform
    void fourierTransform(const std::vector<float>& input, std::vector<float>& outputReal, std::vector<float>& outputImaginary);
    
    //! The complex-to-complex forward Fourier transform
    void fourierTransformComplex(const std::vector<std::complex<float>>& input, std::vector<std::complex<float>>& output);
    
    //! The complex-to-complex forward Fourier transform
    void fourierTransformComplex(const std::vector<float>& inputReal, const std::vector<float>& inputImaginary, std::vector<float>& outputReal, std::vector<float>& outputImaginary);
    
    //! The real-to-complex forward Fourier transform
    Spectrum<float> fourierTransform(const std::vector<float>& input);
    
    //! The complex-to-complex forward Fourier transform
    std::vector<std::complex<float>> fourierTransformComplex(const std::vector<std::complex<float>>& input);
    
    //! The complex-to-real inverse Fourier transform
    void inverseFourierTransform(const std::vector<std::complex<float>>& input, std::vector<float>& output);
    
    //! The complex-to-real inverse Fourier transform
    void inverseFourierTransform(const std::vector<float>& inputReal, const std::vector<float>& inputImaginary, std::vector<float>& output);
    
    //! The complex-to-complex inverse Fourier transform
    void inverseFourierTransformComplex(const std::vector<std::complex<float>>& input, std::vector<std::complex<float>>& output);
    
    //! the complex-to-complex inverse Fourier transform
    void inverseFourierTransformComplex(const std::vector<float>& inputReal, const std::vector<float>& inputImaginary, std::vector<float>& outputReal, std::vector<float>& outputImaginary);
    
    //! The complex-to-real inverse Fourier transform
    std::vector<float> inverseFourierTransform(const std::vector<std::complex<float>>& input);
    
    //! The complex-to-complex inverse Fourier transform
    std::vector<std::complex<float>> inverseFourierTransformComplex(const std::vector<std::complex<float>>& input);
    
    //! The short-time Fourier transform
    std::vector<std::vector<std::complex<float>>> shortTimeFourierTransform(const std::vector<float>& input, FastFourierTransformBase& fourier, const std::vector<float>& window, size_t hopSize);
    
    //! The short-time Fourier transform
    std::vector<std::vector<std::complex<float>>> shortTimeFourierTransform(const std::vector<float>& input, size_t frameSize, const std::vector<float>& window, size_t hopSize);
}

#endif
