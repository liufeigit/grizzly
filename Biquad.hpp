//
//  Biquad.hpp
//  libbear
//
//  Created by Stijn on 21/06/2015.
//  Copyright (c) 2015 FrisHertz. All rights reserved.
//

#ifndef BEAR_DSP_BIQUAD_HPP
#define BEAR_DSP_BIQUAD_HPP

#include "BiquadCoefficients.hpp"
#include "Filter.hpp"

namespace bear::dsp
{
    //! A biquad using Direct Form I
    /*! Biquad that computes samples using the Direct Form I topology */
    template <class T, class CoeffType = double>
    class BiquadDirectFormI : public Filter<T>
    {
    public:
        //! Compute a sample
        constexpr T process(const T& x) override final
        {
            const auto y = x * coefficients.a0 + xz1 * coefficients.a1 + xz2 * coefficients.a2 - coefficients.b1 * yz1 - coefficients.b2 * yz2;
            
            xz2 = xz1;
            xz1 = x;
            
            yz2 = yz1;
            yz1 = y;
            
            return y;
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
    
    //! The default biquad is a DFI biquad
    template <class T, class CoeffType = double>
    using Biquad = BiquadDirectFormI<T, CoeffType>;
}

#endif
