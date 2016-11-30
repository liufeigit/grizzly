//
//  Biquad.hpp
//  libbear
//
//  Created by Stijn on 21/06/2015.
//  Copyright (c) 2015 FrisHertz. All rights reserved.
//

#ifndef GRIZZLY_BIQUAD_HPP
#define GRIZZLY_BIQUAD_HPP

#include "BiquadCoefficients.hpp"

namespace dsp
{
    //! A biquad using Direct Form I
    /*! Biquad that computes samples using the Direct Form I topology */
    template <class T, class CoeffType = double>
    class BiquadDirectFormI
    {
    public:
        //! Compute a sample
        constexpr T process(const T& x)
        {
            const auto y = x * coefficients.a0 + xz1 * coefficients.a1 + xz2 * coefficients.a2 - coefficients.b1 * yz1 - coefficients.b2 * yz2;
            
            xz2 = xz1;
            xz1 = x;
            
            yz2 = yz1;
            yz1 = y;
            
            return y;
        }
        
        //! Compute a sample
        constexpr T operator()(const T& x)
        {
            return process(x);
        }
        
    public:
        //! The coefficients to the biquad
        BiquadCoefficients<CoeffType> coefficients;

    private:
        T xz1 = 0; //!< 1-sample input delay
        T xz2 = 0; //!< 2-sample input delay
        T yz1 = 0; //!< 1-sample output delay
        T yz2 = 0; //!< 2-sample output delay
    };
    
    //! A biquad using Transposed Direct Form II
    /*! Biquad that computes samples using the Direct Form I topology */
    template <class T, class CoeffType = double>
    class BiquadTransposedDirectFormII
    {
    public:
        //! Compute a sample
        constexpr T process(const T& x)
        {
            const auto y = x * coefficients.a0 + z1;
            z1 = x * coefficients.a1 + y * -coefficients.b1 + z2;
            z2 = x * coefficients.a2 + y * -coefficients.b2;
 
            return y;
        }
        
        //! Compute a sample
        constexpr T operator()(const T& x)
        {
            return process(x);
        }
        
    public:
        //! The coefficients to the biquad
        BiquadCoefficients<CoeffType> coefficients;
        
    private:
        T z1 = 0; //!< 1-sample delay
        T z2 = 0; //!< 2-sample delay
    };
}

#endif
