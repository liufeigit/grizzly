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

#ifndef GRIZZLY_HILBERT_TRANSFORM_HPP
#define GRIZZLY_HILBERT_TRANSFORM_HPP

#include <algorithm>
#include <complex>
#include <dsperados/math/statistics.hpp>
#include <dsperados/math/spline.hpp>
#include <iterator>
#include <vector>

#include "FastFourierTransform.hpp"
#include "Spectrum.hpp"

namespace dsp
{
    //! The direction in which a hilbert transform can be done
    enum class HilbertTransformDirection { FORWARD, INVERSE };
    
    //! The Hilbert transform fo a complex signal
    template <typename InputIterator, typename OutputIterator>
    void hilbertTransformComplex(InputIterator begin, InputIterator end, OutputIterator outBegin, HilbertTransformDirection direction)
    {
        // Take the forward Fourier
        const auto size = std::distance(begin, end);
        FastFourierTransform fft(size);
        auto spectrum = fft.forwardComplex(begin);
        
        // Multiply the first half with -j1 (or j1 for inverse)
        const auto halfSize = spectrum.size() / 2;
        for (auto i = 0; i < halfSize; ++i)
            spectrum[i] *= std::complex<float>(0, (direction == HilbertTransformDirection::INVERSE) ? 1 : -1);
        
        // Multiply the second half with j1 (or -j1 for inverse)
        for (auto i = halfSize; i < spectrum.size(); ++i)
            spectrum[i] *= std::complex<float>(0, (direction == HilbertTransformDirection::INVERSE) ? -1 : 1);
        
        // Return the inverse fourier
        fft.inverseComplex(spectrum.begin(), outBegin);
    }
    
    //! The Hilbert transform of a real signal
    template <typename InputIterator, typename OutputIterator>
    void hilbertTransform(InputIterator begin, InputIterator end, OutputIterator outBegin, HilbertTransformDirection direction)
    {
        std::vector<std::complex<typename InputIterator::value_type>> complex(begin, end);
        hilbertTransformComplex(complex.begin(), complex.end(), complex.begin(), direction);
        
        transform(complex.begin(), complex.end(), outBegin, [](const auto& x){ return x.real(); });
    }
    
    //! Find the intrinsic mode function inside a signal
    template <class Iterator>
    std::vector<typename Iterator::value_type> findIntrinsicModeFunction(Iterator begin, Iterator end)
    {
        // This function will keep on sifting the input, with each iteration stored in this vector
        std::vector<typename Iterator::value_type> sift(begin, end);
        
        // Keep on sifting until we've reached the stopping point
        while (true)
        {
            // Find the local minima, maxima and count the zero crossings
            auto minima = math::findLocalMinimaPositions(sift.begin(), sift.end());
            auto maxima = math::findLocalMaximaPositions(sift.begin(), sift.end());
            auto crossings = math::countZeroCrossings(sift.begin(), sift.end());
            
            // Are we done sifting?
            if (minima.size() + maxima.size() - (std::ptrdiff_t)crossings <= 1)
                return sift;
            
            // Create a spline following the valleys
            math::CubicSpline minimaSpline;
            minimaSpline.emplace<size_t>(minima, sift);
            auto minimaSignal = minimaSpline.span(0, sift.size());
            
            // Create a spline following the peaks
            math::CubicSpline maximaSpline;
            maximaSpline.emplace<size_t>(maxima, sift);
            auto maximaSignal = maximaSpline.span(0, sift.size());
            
            // Compute the signal inbetween the peaks and values
            std::vector<typename Iterator::value_type> mean(minimaSignal.size());
            std::transform(minimaSignal.begin(), minimaSignal.end(), maximaSignal.begin(), mean.begin(), [](const float& a, const float& b) { return (a - b) * 0.5; });
            
            // Subtract the mean from the sift
            std::transform(sift.begin(), sift.end(), mean.begin(), sift.begin(), std::minus<>());
        }
    }
    
    //! The result of findIntrinsicModeFunctions
    template <class T>
    struct IntrinsicModeFunctions
    {
        //! The intrinsic mode functions
        std::vector<std::vector<T>> intrinsicModeFunctions;
        
        //! The residue
        std::vector<T> residue;
    };
    
    //! Find all intrinsic mode functions and a residue in a signal
    template <typename Iterator>
    IntrinsicModeFunctions<typename Iterator::value_type> findIntrinsicModeFunctions(Iterator begin, Iterator end)
    {
        IntrinsicModeFunctions<typename Iterator::value_type> result;
        
        result.residue.resize(std::distance(begin, end));
        std::copy(begin, end, result.residue.begin());
        
        // Keep on sifting until the rms of the residue is too small to nice
        while (math::rootMeanSquare<typename Iterator::value_type>(result.residue.begin(), result.residue.end()) >= 0.01)
        {
            result.intrinsicModeFunctions.emplace_back(findIntrinsicModeFunction(result.residue.begin(), result.residue.end()));
            std::transform(result.residue.begin(), result.residue.end(), result.intrinsicModeFunctions.back().begin(), result.residue.begin(), std::minus<>());
        }
        
        return result;
    }
}

#endif
