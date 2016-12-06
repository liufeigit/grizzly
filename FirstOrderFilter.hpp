/*
 
 This file is a part of Grizzly, a modern C++ library for digital signal
 processing. See https://github.com/dsperados/grizzly for more information.
 
 Copyright (C) 2016 Dsperados <info@dsperados.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 --------------------------------------------------------------------
 
 If you would like to use Grizzly for commercial or closed-source
 purposes, please contact us for a commercial license.
 
 */

#ifndef GRIZZLY_FIRST_ORDER_FILTER_HPP
#define GRIZZLY_FIRST_ORDER_FILTER_HPP

#include "FirstOrderCoefficients.hpp"

namespace dsp
{
    //! A first-order, one-pole/one-zero filter (6bd/oct roll-off)
    template <class T, class CoeffType = double>
    class FirstOrderFilter
    {
    public:
        //! Compute a sample
        void write(const T& x)
        {
            // Compute the output
            y = coefficients.a0 * x + coefficients.a1 * xz1 + coefficients.b1 * yz1;
            
            // Update the delays
            xz1 = x;
            yz1 = y;
        }
        
        //! Read the last computed value
        T read() const { return y; }
        
        //! Set the filter state
        void setState(const T& state)
        {
            xz1 = state;
            yz1 = state;
            y = state;
        }
        
        //! Clear the delay elements
        void reset()
        {
            setState(0);
        }
        
    public:
        //! The filter coefficients
        FirstOrderCoefficients<CoeffType> coefficients;
        
    private:
        //! The previous input z^-1
        T xz1 = 0;
        
        //! The previous output z^-1
        T yz1 = 0;
        
        //! The output
        T y = 0;
    };
}


#endif /* GRIZZLY_FIRST_ORDER_FILTER_HPP */
