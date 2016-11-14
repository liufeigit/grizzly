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

#include "FourierTransform.hpp"
#include "Parallel.hpp"

using namespace std;

namespace bear::dsp
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
    
    vector<Spectrum<float>> shortTimeFourierTransform(const vector<float>& input, size_t frameSize, const vector<float>& window, size_t hopSize)
    {
        return shortTimeFourierTransform(input, getFastFourierTransform(frameSize), window, hopSize);
    }
    
    vector<Spectrum<float>> shortTimeFourierTransform(const vector<float>& input, FastFourierTransformBase& fourier, const vector<float>& window, size_t hopSize)
    {
        const auto frameSize = fourier.getSize();
        
        const vector<float>* theWindow = &window;
        
        // Zero-pad the window
        if (window.size() < frameSize)
        {
            static vector<float> w(frameSize, 0.0f);
            auto offset = (frameSize - window.size()) / 2.0;
            
            for (auto i = 0; i < window.size(); ++i)
                w[offset + i] = window[i];
            
            theWindow = &w;
            // Or throw if the window is bigger than the frame size
        } else if (frameSize < window.size()) {
            throw runtime_error("Window size can't be bigger than frame size ()");
        }
        
        // The resulting spectra will be placed here
        vector<Spectrum<float>> spectrum;

        size_t i = 0;
        while (true)
        {
            // Is there enough in the input left?
            if (i + frameSize < input.size())
            {
                // Take a frame
                vector<float> frame(input.begin() + i, input.begin() + i + frameSize);
                
                // Multiply the frame by a window
                auto windowedFrame = multiply(frame, *theWindow);
                
                // Take the transform of the frame and place it in the spectrum vector
                spectrum.emplace_back(fourier.forward(windowedFrame));
            } else {
                vector<float> frame(frameSize, 0.0f);
                copy_n(&input[i], input.size() - i, frame.data());
                
                // Multiply the frame by a window
                auto windowedFrame = multiply(frame, *theWindow);
                
                // Take the transform of the frame and place it in the spectrum vector
                spectrum.emplace_back(fourier.forward(windowedFrame));
                
                // This was the last frame. Bail out of the while loop
                break;
            }
            
            // Hop
            i += hopSize;
        }
        
        return spectrum;
    }
}
