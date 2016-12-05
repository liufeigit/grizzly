//
//  FourierTransform.cpp
//  libbear
//
//  Created by Stijn on 05/11/2015.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <dsperados/math/access.hpp>

#include "FastFourierTransform.hpp"
#include "FourierTransform.hpp"

using namespace std;

namespace dsp
{
    vector<Spectrum<float>> shortTimeFourierTransform(const vector<float>& input, size_t frameSize, const vector<float>* window, size_t hopSize)
    {
        FastFourierTransform fft(frameSize);
        return shortTimeFourierTransform(input, fft, window, hopSize);
    }
    
    vector<Spectrum<float>> shortTimeFourierTransform(const vector<float>& input, FastFourierTransformBase& fourier, const vector<float>* window, size_t hopSize)
    {
        const auto frameSize = fourier.getSize();
        
        if (window && frameSize != window->size())
            throw runtime_error("Frame size not equal to window size");
        
        size_t numberOfFramesRequired = ceil(input.size() / hopSize);
        
        vector<Spectrum<float>> Spectra;
        Spectra.reserve(numberOfFramesRequired);
        
        size_t i = 0;
        while (i < input.size())
        {
            std::vector<float> vec;
            if (i + frameSize < input.size())
                vec.insert(vec.begin(), input.begin() + i, input.begin() + i + frameSize);
            else
            {
                vec.insert(vec.begin(), input.begin() + i, input.end());
                vec.resize(frameSize);
            }
            
            if (window)
                std::transform(vec.begin(), vec.end(), window->begin(), vec.begin(), [](const float& lhs, const float& rhs){ return lhs * rhs; });
            
            Spectra.emplace_back(fourier.forward(vec.data()));
            
            i += hopSize;
        }
        
        return Spectra;
    }
}
