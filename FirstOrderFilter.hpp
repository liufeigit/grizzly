//
//  FirstOrderFilter.hpp
//  bear-dsp
//
//  Created by Milan van der Meer on 30/06/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef GRIZZLY_FIRST_ORDER_FILTER_HPP
#define GRIZZLY_FIRST_ORDER_FILTER_HPP

#include "FirstOrderCoefficients.hpp"

namespace dsp
{
    //! A First-order, one-pole/one-zero filter (6bd/oct roll-off)
    template <class T, class CoeffType = double>
    class FirstOrderFilter
    {
    public:
        //! Compute a sample
        constexpr T process(const T& x)
        {
            // Compute the output
            const auto output = coefficients.a0 * x + coefficients.a1 * xz1 + coefficients.b1 * yz1;
            
            // Update the delays
            xz1 = x;
            yz1 = output;
            
            // Return the outputs
            return output;
        }
        
        //! Set the filter state
        void setState(const T& state)
        {
            xz1 = state;
            yz1 = state;
        }
        
        //! Clear the delay elements
        void clear()
        {
            setState(0);
        }
        
        //! Compute a sample
        constexpr T operator()(const T& x)
        {
            return process(x);
        }
        
    public:
        //! The filter coefficients
        FirstOrderCoefficients<CoeffType> coefficients;
        
    private:
        //! The previous input z^-1
        T xz1 = 0;
        
        //! The previous output z^-1
        T yz1 = 0;
    };
}


#endif /* GRIZZLY_FIRST_ORDER_FILTER_HPP */
