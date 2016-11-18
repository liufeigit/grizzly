//
//  Spectrum.hpp
//  grizzly
//
//  Created by Milan van der Meer on 09/11/16.
//
//

#ifndef Spectrum_hpp
#define Spectrum_hpp

#include <algorithm>
#include <complex>
#include <stdexcept>
#include <vector>

#include <dsperados/math/constants.hpp>
#include <unit/radian.hpp>

namespace bear::dsp
{
    //! Spectrum
    template <class T>
    class Spectrum
    {
    public:
        using Bin = std::complex<T>;
        
    public:
        //! Default constructor
        Spectrum() = default;
        
        //! Construct a spectrum with a vector of bins
        Spectrum(const std::vector<Bin>& spectrum) : data(spectrum)
        {
        }
        
        //! Return the real spectrum
        auto real() const
        {
            std::vector<T> real(data.size());
            std::transform(data.begin(), data.end(), real.begin(), [&](auto bin){ return bin.real(); });
            return real;
        }
        
        //! Return the imaginary spectrum
        auto imaginary() const
        {
            std::vector<T> imaginary(data.size());
            std::transform(data.begin(), data.end(), imaginary.begin(), [&](auto bin){ return bin.imag(); });
            return imaginary;
        }
        
        //! return the magnitudes
        auto magnitudes() const
        {
            std::vector<T> magnitudes(data.size());
            std::transform(data.begin(), data.end(), magnitudes.begin(), [&](auto bin){ return std::abs(bin); });
            return magnitudes;
        }
        
        //! Return the phases
        auto phases() const
        {
            std::vector<unit::radian<T>> phases(data.size());
            std::transform(data.begin(), data.end(), phases.begin(), [&](auto bin){ return std::arg(bin); });
            return phases;
        }
        
        //! Return the unwrapped phases
        auto unwrappedPhases() const
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
        void repplaceImaginaryData(const std::vector<T>& imaginary)
        {
            if (imaginary.size() != data.size())
                throw std::runtime_error("Sizes not equal");
            
            std::transform(data.begin(), data.end(), imaginary.begin(), data.begin(), [](std::complex<T> lhs, T rhs) { lhs.imag(rhs); });
        }
        
        //! Replace the magnitudes of the spectrum
        void replaceMagnitudes(const std::vector<T>& magnitudes)
        {
            if (magnitudes.size() != data.size())
                throw std::runtime_error("Sizes not equal");
            
            for (auto bin = 0; bin < data.size(); ++bin)
                data[bin] = std::polar(magnitudes[bin], std::arg(data[bin]));
        }
        
        //! Replace the phases of the spectrum
        void replacePhases(const std::vector<T>& phases)
        {
            if (phases.size() != data.size())
                throw std::runtime_error("Sizes not equal");
            
            for (auto bin = 0; bin < data.size(); ++bin)
                data[bin] = std::polar(std::abs(data[bin]), phases[bin]);
        }
        
        //! Return the size of the spectrum
        auto size() const { return data.size(); }
        
        //! Return iterator at the begin of the spectrum
        auto begin(){ return data.begin(); }
        
        //! Return iterator at the end of the spectrum
        auto end(){ return data.end(); }
        
        //! Return const iterator at the begin of the spectrum
        auto begin() const { return data.begin(); }
        
        //! Return const iterator at the end of the spectrum
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

#endif /* Spectrum_hpp */
