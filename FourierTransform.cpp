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
        getFastFourierTransform(input.size()).forward(input, output.data);
    }
    
    void fourierTransform(const vector<float>& input, vector<float>& outputReal, vector<float>& outputImaginary)
    {
        getFastFourierTransform(input.size()).forward(input, outputReal, outputImaginary);
    }
    
    void fourierTransformComplex(const Spectrum<float>& input, Spectrum<float>& output)
    {
        getFastFourierTransform(input.size()).forwardComplex(input.data, output.data);
    }
    
    void fourierTransformComplex(const vector<float>& inputReal, const vector<float>& inputImaginary, vector<float>& outputReal, vector<float>& outputImaginary)
    {
        getFastFourierTransform(inputReal.size()).forwardComplex(inputReal, inputImaginary, outputReal, outputImaginary);
    }
    
    Spectrum<float> fourierTransform(const vector<float>& input)
    {
        return {getFastFourierTransform(input.size()).forward(input)};
    }
    
    Spectrum<float> fourierTransformComplex(const Spectrum<float>& input)
    {
        return {getFastFourierTransform(input.size()).forwardComplex(input.data)};
    }
    
    void inverseFourierTransform(const Spectrum<float>& input, vector<float>& output)
    {
        getFastFourierTransform(output.size()).inverse(input.data, output);
    }
    
    void inverseFourierTransform(const vector<float>& inputReal, const vector<float>& inputImaginary, vector<float>& output)
    {
        getFastFourierTransform(output.size()).inverse(inputReal, inputImaginary, output);
    }
    
    void inverseFourierTransformComplex(const Spectrum<float>& input, Spectrum<float>& output)
    {
        getFastFourierTransform(output.size()).inverseComplex(input.data, output.data);
    }
    
    void inverseFourierTransformComplex(const vector<float>& inputReal, const vector<float>& inputImaginary, vector<float>& outputReal, vector<float>& outputImaginary)
    {
        getFastFourierTransform(outputReal.size()).inverseComplex(inputReal, inputImaginary, outputReal, outputImaginary);
    }
    
    vector<float> inverseFourierTransform(const Spectrum<float>& input)
    {
        return getFastFourierTransform((input.size() - 1) * 2).inverse(input.data);
    }
    
    Spectrum<float> inverseFourierTransformComplex(const Spectrum<float>& input)
    {
        return {getFastFourierTransform(input.size()).inverseComplex(input.data)};
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
            
            Spectra.emplace_back(fourier.forward(vector<float>(vec)));
            
            i += hopSize;
        }
        
        return Spectra;
    }
}
