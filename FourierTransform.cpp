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

using namespace gsl;
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
    
    void fourierTransform(const vector<float>& input, vector<std::complex<float>>& output)
    {
        getFastFourierTransform(input.size()).forward(input, output);
    }
    
    void fourierTransform(const vector<float>& input, vector<float>& outputReal, vector<float>& outputImaginary)
    {
        getFastFourierTransform(input.size()).forward(input, outputReal, outputImaginary);
    }
    
    void fourierTransformComplex(const vector<complex<float>>& input, vector<complex<float>>& output)
    {
        getFastFourierTransform(input.size()).forwardComplex(input, output);
    }
    
    void fourierTransformComplex(const vector<float>& inputReal, const vector<float>& inputImaginary, vector<float>& outputReal, vector<float>& outputImaginary)
    {
        getFastFourierTransform(inputReal.size()).forwardComplex(inputReal, inputImaginary, outputReal, outputImaginary);
    }
    
    vector<complex<float>> fourierTransform(const vector<float>& input)
    {
        return getFastFourierTransform(input.size()).forward(input);
    }
    
    vector<complex<float>> fourierTransformComplex(const vector<complex<float>>& input)
    {
        return getFastFourierTransform(input.size()).forwardComplex(input);
    }
    
    void inverseFourierTransform(const vector<complex<float>>& input, vector<float>& output)
    {
        getFastFourierTransform(output.size()).inverse(input, output);
    }
    
    void inverseFourierTransform(const vector<float>& inputReal, const vector<float>& inputImaginary, vector<float>& output)
    {
        getFastFourierTransform(output.size()).inverse(inputReal, inputImaginary, output);
    }
    
    void inverseFourierTransformComplex(const vector<complex<float>>& input, vector<complex<float>>& output)
    {
        getFastFourierTransform(output.size()).inverseComplex(input, output);
    }
    
    void inverseFourierTransformComplex(const vector<float>& inputReal, const vector<float>& inputImaginary, vector<float>& outputReal, vector<float>& outputImaginary)
    {
        getFastFourierTransform(outputReal.size()).inverseComplex(inputReal, inputImaginary, outputReal, outputImaginary);
    }
    
    vector<float> inverseFourierTransform(const vector<complex<float>>& input)
    {
        return getFastFourierTransform((input.size() - 1) * 2).inverse(input);
    }
    
    vector<complex<float>> inverseFourierTransformComplex(const vector<complex<float>>& input)
    {
        return getFastFourierTransform(input.size()).inverseComplex(input);
    }
    
    vector<vector<complex<float>>> shortTimeFourierTransform(const vector<float>& input, size_t frameSize, const vector<float>& window, size_t hopSize)
    {
        return shortTimeFourierTransform(input, getFastFourierTransform(frameSize), window, hopSize);
    }
    
    vector<vector<complex<float>>> shortTimeFourierTransform(const vector<float>& input, FastFourierTransformBase& fourier, const vector<float>& window, size_t hopSize)
    {
        const auto frameSize = fourier.getSize();
        
        // Zero-pad the window
        if (window.size() < frameSize)
        {
            static vector<float> w(frameSize, 0.0f);
            auto offset = (frameSize - window.size()) / 2.0;
            
            for (auto i = 0; i < window.size(); ++i)
                w[offset + i] = window[i];
            
            window = w;
            // Or throw if the window is bigger than the frame size
        } else if (frameSize < window.size()) {
            throw runtime_error("Window size can't be bigger than frame size ()");
        }
        
        // The resulting spectra will be placed here
        vector<vector<complex<float>>> spectrum;

        size_t i = 0;
        while (true)
        {
            // Is there enough in the input left?
            if (i + frameSize < input.size())
            {
                // Take a frame
                auto frame = input.subspan(i, i + frameSize);
                
                // Multiply the frame by a window
                auto windowedFrame = multiply(frame, window);
                
                // Take the transform of the frame and place it in the spectrum vector
                spectrum.emplace_back(fourier.forward(windowedFrame));
            } else {
                vector<float> frame(frameSize, 0.0f);
                copy_n(&input[i], input.size() - i, frame.data());
                
                // Multiply the frame by a window
                auto windowedFrame = multiply(frame, window);
                
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
