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
#include <unit/radian.hpp>

namespace bear
{
    //! Generate a curve from a range
    /*! @param transferFunction A unary function which transforms x -> y for each entry point
     @param size Number of points in the curve */
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
    
    //! Apply z-transform on a input sequence and return the transfer function
    template <typename T1, typename T2>
    static inline auto zTransform(gsl::span<T1> input)
    {
        return [=](const unit::radian<T2>& angularFrequency)
        {
            std::complex<T2> accumulator(0, 0);
            for (auto order = 0; order < input.size(); ++order)
                accumulator += input[order] * std::polar(1, -angularFrequency * order);
            
            return accumulator;
        };
    }
    
    //! Return the frequency response of a transfer function (H(z)) for a single frequency
    /*! @param angularFrequency The frequency in radians
     @param transferFunction The transfer function (H(z)) of an impulse response
     */
    template <typename T1, typename T2>
    static inline auto frequencyResponse(const unit::radian<T1>& angularFrequency, std::function<std::complex<T2>(T2)> transferFunction)
    {
        return transferFunction(angularFrequency);
    }
    
    //! Return the frequency response of an impulse response for a single frequency
    /*! @param angularFrequency The frequency in radians
     @param impulseResponse The system impluse response
     */
    template <typename T1, typename T2>
    static inline auto frequencyResponse(const unit::radian<T1>& angularFrequency, gsl::span<T2> impulseResponse)
    {
        return zTransform(impulseResponse)(angularFrequency);
    }
    
    //! Return the frequency responses of a transfer function (H(z)) for a list of frequencies
    template <typename T1, typename T2>
    static inline auto frequencyResponse(gsl::span<const unit::radian<T1>> angularFrequencies, std::function<std::complex<T2>(T2)> transferFunction)
    {
        std::vector<std::complex<unit::radian<T1>>> output;
        output.reserve(angularFrequencies.size());
        
        for (auto& frequency: angularFrequencies)
            output.emplace_back(frequencyResponse(frequency, transferFunction));
        
        return output;
    }
    
    //! Return the frequency responses of an impulse response for a list of frequencies
    template <typename T1, typename T2>
    static inline auto frequencyResponse(gsl::span<const unit::radian<T1>> angularFrequencies, gsl::span<T2> impulseResponse)
    {
        return frequencyResponse(angularFrequencies, zTransform(impulseResponse));
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
