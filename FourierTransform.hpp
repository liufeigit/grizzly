//
//  FourierTransform.hpp
//  libbear
//
//  Created by Stijn on 04/11/2015.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef GRIZZLY_FOURIER_TRANSFORM_HPP
#define GRIZZLY_FOURIER_TRANSFORM_HPP

#include <complex>
#include <vector>

#include "Spectrum.hpp"

namespace dsp
{
    class FastFourierTransformBase;
    
    //! The real-to-complex forward Fourier transform
    void fourierTransform(const std::vector<float>& input, Spectrum<float>& output);
    
    //! The real-to-complex forward Fourier transform
    void fourierTransform(const std::vector<float>& input, std::vector<float>& outputReal, std::vector<float>& outputImaginary);
    
    //! The complex-to-complex forward Fourier transform
    void fourierTransformComplex(const Spectrum<float>& input, Spectrum<float>& output);
    
    //! The complex-to-complex forward Fourier transform
    void fourierTransformComplex(const std::vector<float>& inputReal, const std::vector<float>& inputImaginary, std::vector<float>& outputReal, std::vector<float>& outputImaginary);
    
    //! The real-to-complex forward Fourier transform
    Spectrum<float> fourierTransform(const std::vector<float>& input);
    
    //! The complex-to-complex forward Fourier transform
    Spectrum<float> fourierTransformComplex(const Spectrum<float>& input);
    
    //! The complex-to-real inverse Fourier transform
    void inverseFourierTransform(const Spectrum<float>& input, std::vector<float>& output);
    
    //! The complex-to-real inverse Fourier transform
    void inverseFourierTransform(const std::vector<float>& inputReal, const std::vector<float>& inputImaginary, std::vector<float>& output);
    
    //! The complex-to-complex inverse Fourier transform
    void inverseFourierTransformComplex(const Spectrum<float>& input, Spectrum<float>& output);
    
    //! the complex-to-complex inverse Fourier transform
    void inverseFourierTransformComplex(const std::vector<float>& inputReal, const std::vector<float>& inputImaginary, std::vector<float>& outputReal, std::vector<float>& outputImaginary);
    
    //! The complex-to-real inverse Fourier transform
    std::vector<float> inverseFourierTransform(const Spectrum<float>& input);
    
    //! The complex-to-complex inverse Fourier transform
    Spectrum<float> inverseFourierTransformComplex(const Spectrum<float>& input);
    
    //! The short-time Fourier transform
    std::vector<Spectrum<float>> shortTimeFourierTransform(const std::vector<float>& input, FastFourierTransformBase& fourier, const std::vector<float>* window, size_t hopSize);
    
    //! The short-time Fourier transform
    std::vector<Spectrum<float>> shortTimeFourierTransform(const std::vector<float>& input, size_t frameSize, const std::vector<float>* window, size_t hopSize);
}

#endif
