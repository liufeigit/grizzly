//
//  Math.hpp
//  bear-core
//
//  Created by Stijn on 17/05/2016.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef BEAR_CORE_MATH_HPP
#define BEAR_CORE_MATH_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <stdexcept>

#include "MathConstants.hpp"

namespace bear
{
    //! Clamp a number within a given range
    /*! @throw std::invalid_argument if max < min */
    template <class T, class U, class V>
    inline static auto clamp(const T& value, const U& min, const V& max)
    {
        if (max < min)
            throw std::invalid_argument("max < min");
        
        return value < min ? min : max < value ? max : value;
    }
    
    //! Clamp a number within a given range (start end = 0)
    /*! @throw std::invalid_argument if max < 0 */
    template <class T, class U>
    inline static std::common_type_t<T, U> clamp(const T& value, const U& max)
    {
        if (max < 0)
            throw std::invalid_argument("max < 0");
        
        return value < 0 ? 0 : max < value ? max : value;
    }
    
    //! Wrap a number within a given range
    /*! @throw std::invalid_argument if max <= min */
    template <class T, class U, class V>
    inline static auto wrap(T value, const U& min, const V& max)
    {
        if (max <= min)
            throw std::invalid_argument("max <= min");
        
        while (max <= value)
            value -= (max - min);
        
        while (value < min)
            value += (max - min);
        
        return value;
    }
    
    //! Wrap a number within a given range, where min = 0
    /*! @throw std::invalid_argument if max <= 0 */
    template <class T, class U>
    inline static auto wrap(T value, const U& max)
    {
        if (max <= 0)
            throw std::invalid_argument("max <= 0");
        
        while (max <= value)
            value -= max;
        
        while (value < 0)
            value += max;
        
        return value;
    }
    
    //! Returns whether an integral is a power of two
    /*! @return false for x == 0 */
    constexpr bool isPowerOf2(size_t x)
    {
        return (x == 0) ? false : (x == 1 || (x & (x - 1)) == 0);
    }
    
    //! Find the smallest power of 2 bigger than a given number
    /*! @return 1 for x == 0 */
    constexpr size_t ceilToPowerOf2(size_t x)
    {
        size_t y = 1;
        while (y < x)
            y <<= 1;
        
        return y;
    }
}

#endif /* BEAR_CORE_MATH_HPP */
