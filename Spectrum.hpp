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

#ifndef GRIZZLY_SPECTRUM_HPP
#define GRIZZLY_SPECTRUM_HPP

#include <algorithm>
#include <complex>
#include <stdexcept>
#include <vector>

#include <dsperados/math/constants.hpp>
#include <unit/radian.hpp>

namespace dsp
{
    //! Spectrum of frequency bins
    template <class T>
    class Spectrum
    {
    public:
        using Bin = std::complex<T>;
        
    public:
        //! Default constructor
        Spectrum() = default;
        
        //! Construct a spectrum with a vector of bins
        Spectrum(const std::vector<Bin>& spectrum) :
            data(spectrum)
        {
            
        }
        
        //! Return the real spectrum
        std::vector<T> real() const
        {
            std::vector<T> real(data.size());
            std::transform(data.begin(), data.end(), real.begin(), [&](auto bin){ return bin.real(); });
            return real;
        }
        
        //! Return the imaginary spectrum
        std::vector<T> imaginary() const
        {
            std::vector<T> imaginary(data.size());
            std::transform(data.begin(), data.end(), imaginary.begin(), [&](auto bin){ return bin.imag(); });
            return imaginary;
        }
        
        //! return the magnitudes
        std::vector<T> magnitudes() const
        {
            std::vector<T> magnitudes(data.size());
            std::transform(data.begin(), data.end(), magnitudes.begin(), [&](auto bin){ return std::abs(bin); });
            return magnitudes;
        }
        
        //! Return the phases
        std::vector<unit::radian<T>> phases() const
        {
            std::vector<unit::radian<T>> phases(data.size());
            std::transform(data.begin(), data.end(), phases.begin(), [&](auto bin){ return std::arg(bin); });
            return phases;
        }
        
        //! Return the unwrapped phases
        std::vector<unit::radian<T>> unwrappedPhases() const
        {
            auto wrappedPhases = phases();
            std::vector<unit::radian<T>> unwrappedPhases(wrappedPhases.size());
            unwrappedPhases.front() = wrappedPhases.front();
            auto previousValue = unwrappedPhases.front();
            
            signed multiplier = 0;
            std::transform(wrappedPhases.begin() + 1, wrappedPhases.end(), unwrappedPhases.begin() + 1, [&](auto value)
            {
                auto currentValue = value;
                               
                if (currentValue - previousValue > math::PI<T>)
                    multiplier--;
                else if (currentValue - previousValue < -math::PI<T>)
                    multiplier++;
               
                previousValue = currentValue;
                return value + math::TWO_PI<T> * multiplier;
            });
            
            return unwrappedPhases;
        }
        
        //! Replace the real data of the spectrum
        void replaceRealData(const std::vector<T>& real)
        {
            if (real.size() != data.size())
                throw std::runtime_error("Sizes not equal");
            
            std::transform(data.begin(), data.end(), real.begin(), data.begin(), [](std::complex<T> lhs, T rhs) { lhs.real(rhs); });
        }
        
        //! Replace the imaginary data of the spectrum
        void replaceImaginaryData(const std::vector<T>& imaginary)
        {
            if (imaginary.size() != data.size())
                throw std::invalid_argument("Sizes not equal");
            
            std::transform(data.begin(), data.end(), imaginary.begin(), data.begin(), [](std::complex<T> lhs, T rhs) { lhs.imag(rhs); });
        }
        
        //! Replace the magnitudes of the spectrum
        void replaceMagnitudes(const std::vector<T>& magnitudes)
        {
            if (magnitudes.size() != data.size())
                throw std::invalid_argument("Sizes not equal");
            
            for (auto bin = 0; bin < data.size(); ++bin)
                data[bin] = std::polar(magnitudes[bin], std::arg(data[bin]));
        }
        
        //! Replace the phases of the spectrum
        void replacePhases(const std::vector<unit::radian<T>>& phases)
        {
            if (phases.size() != data.size())
                throw std::invalid_argument("Sizes not equal");
            
            for (auto bin = 0; bin < data.size(); ++bin)
                data[bin] = std::polar(std::abs(data[bin]), phases[bin].value);
        }
        
        //! Return the size of the spectrum
        auto size() const { return data.size(); }
        
        // Return iterators for ranged for-loops
        auto begin() { return data.begin(); }
        auto begin() const { return data.begin(); }
        auto end() { return data.end(); }
        auto end() const { return data.end(); }
        
        //! Return a single bin in cartesian coordinates
        Bin& operator[](size_t index) { return data[index]; }
        
        //! Return a single const bin in cartesian coordinates
        const Bin& operator[](size_t index) const { return data[index]; }
        
    public:
        //! Spectrum in cartesian coordinates
        std::vector<Bin> data;
    };
}

#endif /* GRIZZLY_SPECTRUM_HPP */
