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
    /*! Biquad that computes samples using the Direct Form I topology.
        This topology gives you less side-effects when chaning coefficients during processing. */
    template <class T, class CoeffType = double>
    class BiquadDirectFormI
    {
    public:
        //! Compute a sample
        void write(const T& x)
        {
            y = x * coefficients.a0 + xz1 * coefficients.a1 + xz2 * coefficients.a2 - coefficients.b1 * yz1 - coefficients.b2 * yz2;
            
            // Update the delays
            xz2 = xz1;
            xz1 = x;
            yz2 = yz1;
            yz1 = y;
        }
        
        //! Insert a new sample in the Biquad
        T read() const { return y; }
        
        //! Set the filter state
        void setState(const T& state)
        {
            xz1 = state;
            yz1 = state;
            xz2 = state;
            yz2 = state;
            y = state;
        }
        
        //! Clear the delay elements
        void reset()
        {
            setState(0);
        }
        
    public:
        //! The coefficients to the biquad
        BiquadCoefficients<CoeffType> coefficients;

    private:
        T y = 0; //! output
        T xz1 = 0; //!< 1-sample input delay
        T xz2 = 0; //!< 2-sample input delay
        T yz1 = 0; //!< 1-sample output delay
        T yz2 = 0; //!< 2-sample output delay
    };
    
    //! A biquad using Transposed Direct Form II
    /*! Biquad that computes samples using the Transposed Direct Form II topology.
        This is supposedly better for floating-point computation, although it has more
        side-effects when you change the coefficients during processing. */
    template <class T, class CoeffType = double>
    class BiquadTransposedDirectFormII
    {
    public:
        //! Insert a new sample in the Biquad
        void write(const T& x)
        {
            y = x * coefficients.a0 + z1;
            
            // Update the delays
            z1 = x * coefficients.a1 + y * -coefficients.b1 + z2;
            z2 = x * coefficients.a2 + y * -coefficients.b2;
        }
        
        //! Insert a new sample in the Biquad
        T read() const { return y; }
        
        //! Set the filter state
        void setState(const T& state)
        {
            z1 = state;
            z1 = state;
            y = state;
        }
        
        //! Clear the delay elements
        void reset()
        {
            setState(0);
        }
        
    public:
        //! The coefficients to the biquad
        BiquadCoefficients<CoeffType> coefficients;
        
    private:
        T y = 0; //! output
        T z1 = 0; //!< 1-sample delay
        T z2 = 0; //!< 2-sample delay
    };
}

#endif
