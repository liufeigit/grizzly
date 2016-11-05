//
//  Transfer.hpp
//  bear-core
//
//  Created by Milan van der Meer on 19/07/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef BEAR_CORE_TRANSFER_HPP
#define BEAR_CORE_TRANSFER_HPP

#include <functional>
#include <gsl/span>
#include <vector>
#include <stdexcept>

#include <dsperados/math/utility.hpp>

namespace bear
{
    //! Generate a curve from a range
    /*! @param range Range for transfer
        @param resolution The number of entry points (equally placed) in range, determines output size
        @param transferFunction A unary function which transforms x -> y for each entry point */
    template <class T>
    std::vector<T> generateCurve(std::function<T(T)> function, size_t size)
    {
        const auto increment = 1.l / size;
        
        std::vector<T> output;
        output.reserve(size);
        
        for (auto i = 0; i < size; ++i)
            output.emplace_back(function(i * increment));
        
        return output;
    }
    
//    template <class T1, class T2>
//    std::vector<T2> generateCurve(const T1 minimum, const T2 maximum, std::size_t resolution, std::function<T2(T2)> transferFunction)
//    {
//        const auto increment = static_cast<long double>(maximum - minimum) / resolution;
//        
//        std::vector<T2> y;
//        y.reserve(resolution);
//        
//        for (auto i = 0; i < resolution; ++i)
//            y.emplace_back(transferFunction((i * increment) + minimum));
//        
//        return y;
//    }
    
    //! Apply z-transform on a input sequence and return the transfer function
    template <typename T>
    static inline auto zTransform(gsl::span<T> input)
    {
        std::vector<T> inputVec(input.begin(), input.end());
        
        return [=](const T& angularFrequency)
        {
            std::complex<T> accumulator(0, 0);
            for (auto order = 0; order < inputVec.size(); ++order)
                accumulator += inputVec[order] * std::polar(1.f, -angularFrequency * order);
            
            return accumulator;
        };
    }
    
    //! Return the frequency response of a transfer function (H(z)) for a single frequency
    /*! @param angularFrequency The frequency in radians
     @param transferFunction The transfer function (H(z)) of an impulse response
     */
    template <typename T>
    static inline auto frequencyResponse(const T& angularFrequency, std::function<std::complex<T>(T)> transferFunction)
    {
        return transferFunction(angularFrequency);
    }
    
    //! Return the frequency response of an impulse response for a single frequency
    /*! @param angularFrequency The frequency in radians
     @param impulseResponse The system impluse response
     */
    template <typename T>
    static inline auto frequencyResponse(const T& angularFrequency, gsl::span<T> impulseResponse)
    {
        return zTransform(impulseResponse)(angularFrequency);
    }
    
    //! Return the frequency responses of a transfer function (H(z)) for a list of frequencies
    template <typename T>
    static inline auto frequencyResponse(gsl::span<T> angularFrequencies, std::function<std::complex<T>(T)> transferFunction)
    {
        std::vector<std::complex<T>> output;
        output.reserve(angularFrequencies.size());
        
        for (auto& frequency: angularFrequencies)
            output.emplace_back(frequencyResponse(frequency, transferFunction));
        
        return output;
    }
    
    //! Return the frequency responses of an impulse response for a list of frequencies
    template <typename T>
    static inline auto frequencyResponse(gsl::span<T> angularFrequencies, gsl::span<T> impulseResponse)
    {
        std::function<std::complex<T>(T)> transferFunction = zTransform(impulseResponse); // Can't use auto or fill in directly as 2nd argument for some reason..?
        return frequencyResponse(angularFrequencies, transferFunction);
    }
    
    // Create an impulse response given a filter and a size
    template <typename T>
    static inline auto createImpulseResponse(std::function<T(T)> filter, size_t size)
    {
        if (size < 1)
            throw std::runtime_error("Size < 1");
        
        std::vector<T> response(size);
        
        response[0] = filter(1);
        for (auto index = 1; index < size; ++index)
            response[index] = filter(0);
        
        return response;
    }
}

#endif
