//
//  Window.hpp
//  libbear
//
//  Created by Stijn on 05/11/2015.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef BEAR_DSP_WINDOW_HPP
#define BEAR_DSP_WINDOW_HPP

#include <vector>

namespace dsp
{
    //! Create a rectangular window
    std::vector<float> createRectangularWindow(size_t size, float amplitude = 1)
    {
        return std::vector<float>(size, amplitude);
    }
    
    //! Fill a triangle window
    void fillTriangleWindow(std::vector<float>& window, double t = 0.5);
    
    //! Create a triangle window
    std::vector<float> createTriangleWindow(size_t size, double t = 0.5);
    
    //! Create a Hann window
    std::vector<float> createHannWindow(size_t size);
    
    //! Create a Hamming window
    std::vector<float> createHammingWindow(size_t size);
    
    //! Create a Blackman window
    std::vector<float> createBlackmanWindow(size_t size);
    
    //! Create a sinc window
    std::vector<float> createSincWindow(size_t size, float normalizedCutOff, float gain);
    
    //! Create a symmetrical Sinc window
    std::vector<float> createSymmetricSincWindow(const size_t size, const double normalizedFrequency);
    
    //! Create a symmetric Hanning window
    std::vector<float> createSymmetricHannWindow(const size_t size);
    
    //! Create a symmetric Kaiser window. The Beta factor (>= 1) determines the steepness
    std::vector<float> createSymmetricKaiserWindow(size_t size, double betaFactor);
}

#endif /* BEAR_DSP_WINDOW_HPP */
