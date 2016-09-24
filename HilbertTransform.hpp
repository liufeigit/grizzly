//
//  HilbertTransform.hpp
//  libbear
//
//  Created by Stijn Frishert on 02/02/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#ifndef BEAR_HILBERT_TRANSFORM_HPP
#define BEAR_HILBERT_TRANSFORM_HPP

#include <complex>
#include <gsl/span.h>
#include <vector>

namespace bear::dsp
{
    //! The Hilbert transform of a real signal
    std::vector<float> hilbertTransform(gsl::span<const float> input, bool inverse = false);
    
    //! The Hilbert transform fo a complex signal
    std::vector<std::complex<float>> hilbertTransformComplex(gsl::span<const std::complex<float>> input, bool inverse = false);
    
    //! Find the intrinsic mode function inside a signal
    std::vector<float> findIntrinsicModeFunction(gsl::span<const float> input);
    
    //! The result of findIntrinsicModeFunctions
    struct IntrinsicModeFunctions
    {
    public:
        //! Construct an empty object
        IntrinsicModeFunctions() = default;
        
        //! Construct by giving the residue
        IntrinsicModeFunctions(gsl::span<const float> residue) : residue(residue.begin(), residue.end()) { }
        
    public:
        //! The intrinsic mode functions
        std::vector<std::vector<float>> intrinsicModeFunctions;
        
        //! The residue
        std::vector<float> residue;
    };
    
    //! Find all intrinsic mode functions and a residue in a signal
    IntrinsicModeFunctions findIntrinsicModeFunctions(gsl::span<const float> input);
}

#endif
