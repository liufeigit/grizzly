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
#include <vector>

#include "Spectrum.hpp"

namespace dsp
{
    //! The Hilbert transform of a real signal
    std::vector<float> hilbertTransform(const std::vector<float>& input, bool inverse = false);
    
    //! The Hilbert transform fo a complex signal
    Spectrum<float> hilbertTransformComplex(const std::vector<std::complex<float>>& input, bool inverse = false);
    
    //! Find the intrinsic mode function inside a signal
    std::vector<float> findIntrinsicModeFunction(const std::vector<float>& input);
    
    //! The result of findIntrinsicModeFunctions
    struct IntrinsicModeFunctions
    {
    public:
        //! Construct an empty object
        IntrinsicModeFunctions() = default;
        
        //! Construct by giving the residue
        IntrinsicModeFunctions(const std::vector<float>& residue) : residue(residue.begin(), residue.end()) { }
        
    public:
        //! The intrinsic mode functions
        std::vector<std::vector<float>> intrinsicModeFunctions;
        
        //! The residue
        std::vector<float> residue;
    };
    
    //! Find all intrinsic mode functions and a residue in a signal
    IntrinsicModeFunctions findIntrinsicModeFunctions(const std::vector<float>& input);
}

#endif
