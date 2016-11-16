//
//  Normalise.hpp
//  grizzly
//
//  Created by Milan van der Meer on 16/11/16.
//
//

#ifndef Normalise_hpp
#define Normalise_hpp

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace bear::dsp
{
    //! Normalize the sum of a function to a value (e.g. 1, 1 --> 0.5, 0.5 for sum = 1)
    void normalizeSum(std::vector<float>& x, double sum = 1.0);
    
    //! Normalize a function to its highest absolute peak
    void normalizeBiDirectional(std::vector<float>& x, double peakMaximum = 1.0);
    
    //! Normalize a function to its highest peak
    void normalizeUniDirectional(std::vector<float>& x, double peakMaximum = 1.0);
    
    void normalizeSum(std::vector<float>& x, double sum)
    {
        double gainFactor = 1.0 / sum;
        double accumulator = 0;
        
        for (auto& value : x)
            accumulator += value;
        
        if (accumulator) // OF RUN TIME ERROR?!
            for (auto& value : x)
                value /= (accumulator * gainFactor);
    }
    
    void normalizeBiDirectional(std::vector<float>& x, double peakMaximum)
    {
        double highest = 0;
        
        for (auto& value : x)
            if (fabs(value) > highest) highest = fabs(value);
        
        double gainFactor = fabs(peakMaximum) / highest;
        
        for (auto& value : x)
            value *= gainFactor;
    }
    
    void normalizeUniDirectional(std::vector<float>& x, double peakMaximum)
    {
        if (peakMaximum <= 0)
            throw std::runtime_error("Peak maximum has to be higher than zero. Use normalizeBilateral instead?");
        
        double highest = 0;
        
        if (!highest)
            throw std::runtime_error("There has to be at least one point above zero. Use normalizeBilateral instead?");
        
        for (auto& value : x)
            if (value > highest) highest = value;
        
        double gainFactor = peakMaximum / highest;
        
        for (auto& value : x)
            value *= gainFactor;
    }
}

#endif /* Normalise_hpp */
