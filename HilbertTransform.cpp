//
//  HilbertTransform.cpp
//  libbear
//
//  Created by Stijn Frishert on 02/02/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#include "FourierTransform.hpp"
#include "HilbertTransform.hpp"

using namespace math;
using namespace std;

namespace dsp
{
    vector<float> hilbertTransform(const vector<float>& input, bool inverse)
    {
        vector<std::complex<float>> complexInput(input.size());
        transform(input.begin(), input.end(), complexInput.begin(), [](const auto& x){ return x; });

        auto hilbert = hilbertTransformComplex(complexInput, inverse);

        vector<float> result(hilbert.size());
        transform(hilbert.begin(), hilbert.end(), result.begin(), [](const auto& x){ return x.real(); });

        return result;
    }

    Spectrum<float> hilbertTransformComplex(const vector<std::complex<float>>& input, bool inverse)
    {
        // Take the forward Fourier
        auto spectrum = fourierTransformComplex(input);

        const auto halfSize = spectrum.size() / 2;

        // Multiply the first half with -j1 (or j1 for inverse)
        for (auto i = 0; i < halfSize; ++i)
            spectrum[i] *= std::complex<float>(0, inverse ? 1 : -1);

        // Multiply the second half with j1 (or -j1 for inverse)
        for (auto i = halfSize; i < spectrum.size(); ++i)
            spectrum[i] *= std::complex<float>(0, inverse ? -1 : 1);

        // Return the inverse fourier
        return {inverseFourierTransformComplex(spectrum)};
    }
}
