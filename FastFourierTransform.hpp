//
//  FastFourierTransform.hpp
//  grizzly
//
//  Created by Stijn Frishert on 11/28/16.
//
//

#ifndef GRIZZLY_FAST_FOURIER_TRANSFORM_HPP
#define GRIZZLY_FAST_FOURIER_TRANSFORM_HPP

#include "Ooura/FastFourierTransformOoura.hpp"

#ifdef __APPLE__
    #include "Apple/FastFourierTransformAccelerate.hpp"

    namespace dsp { using FastFourierTransform = FastFourierTransformAccelerate; }
#else
    namespace dsp { using FastFourierTransform = FastFourierTransformOoura; }
#endif


#endif /* GRIZZLY_FAST_FOURIER_TRANSFORM_HPP */
