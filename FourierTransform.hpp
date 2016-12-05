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
    
    //! The short-time Fourier transform
    std::vector<Spectrum<float>> shortTimeFourierTransform(const std::vector<float>& input, FastFourierTransformBase& fourier, const std::vector<float>* window, size_t hopSize);
    
    //! The short-time Fourier transform
    std::vector<Spectrum<float>> shortTimeFourierTransform(const std::vector<float>& input, size_t frameSize, const std::vector<float>* window, size_t hopSize);
}

#endif
