//
//  FirstOrderFilter.hpp
//  bear-dsp
//
//  Created by Milan van der Meer on 30/06/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef DMS_DSP_FIRST_ORDER_FILTERS_HPP
#define DMS_DSP_FIRST_ORDER_FILTERS_HPP

#include "Filter.hpp"
#include "FirstOrderCoefficients.hpp"

namespace dsp
{
    //! A First-order, one-pole/one-zero filter (6bd/oct roll-off)
    template <class T, class CoeffType = double>
    class FirstOrderFilter : public Filter<T>
    {
    public:
        //! Compute a sample
        constexpr T process(const T& x) override final
        {
            // Compute the output
            const auto output = coefficients.a0 * x + coefficients.a1 * xz1 + coefficients.b1 * yz1;
            
            // Update the delays
            xz1 = x;
            yz1 = output;
            
            // Return the outputs
            return output;
        }
        
        //! Clear the delay elements
        void clear()
        {
            xz1 = 0;
            yz1 = 0;
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


#endif /* DMS_DSP_FIRST_ORDER_FILTERS_HPP */
