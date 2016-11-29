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
    FastFourierTransform& getFastFourierTransform(size_t size)
    {
        static unordered_map<size_t, unique_ptr<FastFourierTransform>> transforms;
        
        auto it = transforms.find(size);
        if (it != transforms.end())
            return *it->second;
        
        return *transforms.emplace(size, make_unique<FastFourierTransform>(size)).first->second;
    }
    
    void fourierTransform(const vector<float>& input, Spectrum<float>& output)
    {
        getFastFourierTransform(input.size()).forward(input.data(), output.begin());
    }
    
    void fourierTransform(const vector<float>& input, vector<float>& outputReal, vector<float>& outputImaginary)
    {
        getFastFourierTransform(input.size()).forward(input.data(), outputReal.data(), outputImaginary.data());
    }
    
    void fourierTransformComplex(const Spectrum<float>& input, Spectrum<float>& output)
    {
        getFastFourierTransform(input.size()).forwardComplex(input.begin(), output.begin());
    }
    
    void fourierTransformComplex(const vector<float>& inputReal, const vector<float>& inputImaginary, vector<float>& outputReal, vector<float>& outputImaginary)
    {
        getFastFourierTransform(inputReal.size()).forwardComplex(inputReal.data(), inputImaginary.data(), outputReal.data(), outputImaginary.data());
    }
    
    Spectrum<float> fourierTransform(const vector<float>& input)
    {
        return {getFastFourierTransform(input.size()).forward(input.data())};
    }
    
    Spectrum<float> fourierTransformComplex(const Spectrum<float>& input)
    {
        return {getFastFourierTransform(input.size()).forwardComplex(input.begin())};
    }
    
    void inverseFourierTransform(const Spectrum<float>& input, vector<float>& output)
    {
        getFastFourierTransform(output.size()).inverse(input.begin(), output.data());
    }
    
    void inverseFourierTransform(const vector<float>& inputReal, const vector<float>& inputImaginary, vector<float>& output)
    {
        getFastFourierTransform(output.size()).inverse(inputReal.data(), inputImaginary.data(), output.data());
    }
    
    void inverseFourierTransformComplex(const Spectrum<float>& input, Spectrum<float>& output)
    {
        getFastFourierTransform(output.size()).inverseComplex(input.begin(), output.begin());
    }
    
    void inverseFourierTransformComplex(const vector<float>& inputReal, const vector<float>& inputImaginary, vector<float>& outputReal, vector<float>& outputImaginary)
    {
        getFastFourierTransform(outputReal.size()).inverseComplex(inputReal.data(), inputImaginary.data(), outputReal.data(), outputImaginary.data());
    }
    
    vector<float> inverseFourierTransform(const Spectrum<float>& input)
    {
        return getFastFourierTransform((input.size() - 1) * 2).inverse(input.begin());
    }
    
    Spectrum<float> inverseFourierTransformComplex(const Spectrum<float>& input)
    {
        return {getFastFourierTransform(input.size()).inverseComplex(input.begin())};
    }
    
    vector<Spectrum<float>> shortTimeFourierTransform(const vector<float>& input, size_t frameSize, const vector<float>* window, size_t hopSize)
    {
        return shortTimeFourierTransform(input, getFastFourierTransform(frameSize), window, hopSize);
    }
    
    vector<Spectrum<float>> shortTimeFourierTransform(const vector<float>& input, FastFourierTransformBase& fourier, const vector<float>* window, size_t hopSize)
    {
        const auto frameSize = fourier.getSize();
        
        if (frameSize != window->size())
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
