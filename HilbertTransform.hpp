//
//  HilbertTransform.hpp
//  libbear
//
//  Created by Stijn Frishert on 02/02/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#ifndef BEAR_HILBERT_TRANSFORM_HPP
#define BEAR_HILBERT_TRANSFORM_HPP

#include <algorithm>
#include <complex>
#include <dsperados/math/statistics.hpp>
#include <dsperados/math/spline.hpp>
#include <iterator>
#include <vector>

#include "Spectrum.hpp"

namespace dsp
{
    //! The Hilbert transform of a real signal
    std::vector<float> hilbertTransform(const std::vector<float>& input, bool inverse = false);
    
    //! The Hilbert transform fo a complex signal
    Spectrum<float> hilbertTransformComplex(const std::vector<std::complex<float>>& input, bool inverse = false);
    
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
