//
//  HilbertTransform.cpp
//  libbear
//
//  Created by Stijn Frishert on 02/02/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#include <algorithm>

#include "FourierTransform.hpp"
#include "HilbertTransform.hpp"
#include "Parallel.hpp"
#include "Spline.hpp"

using namespace gsl;
using namespace std;

namespace bear::dsp
{
    vector<float> hilbertTransform(span<const float> input, bool inverse)
    {
        vector<std::complex<float>> complexInput(input.size());
        transform(input.begin(), input.end(), complexInput.begin(), [](const auto& x){ return x; });

        auto hilbert = hilbertTransformComplex(complexInput, inverse);

        vector<float> result(hilbert.size());
        transform(hilbert.begin(), hilbert.end(), result.begin(), [](const auto& x){ return x.real(); });

        return result;
    }

    vector<std::complex<float>> hilbertTransformComplex(span<const std::complex<float>> input, bool inverse)
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
        return inverseFourierTransformComplex(spectrum);
    }
    
    

    vector<float> findIntrinsicModeFunction(span<const float> input)
    {
        vector<float> sift(input.begin(), input.end());

        while (true)
        {
            auto minima = localMinima(span<const float>(sift));
            auto maxima = localMaxima(span<const float>(sift));
            auto crossings = zeroCrossings(span<const float>(sift));

            if (minima.size() + maxima.size() - (int)crossings <= 1)
                return sift;

            CubicSpline minimaSpline;
            minimaSpline.emplace<size_t>(minima, sift);

            auto minimaSignal = minimaSpline.span(0, sift.size());

            CubicSpline maximaSpline;
            maximaSpline.emplace<size_t>(maxima, sift);

            auto maximaSignal = maximaSpline.span(0, sift.size());

            auto m = mean(span<const float>(minimaSignal), span<const float>(maximaSignal));

            sift = subtract(span<const float>(sift), span<const float>(m));
        }
    }

   IntrinsicModeFunctions findIntrinsicModeFunctions(span<const float> input)
    {
        IntrinsicModeFunctions result;

        result.residue.resize(input.size());
        copy(input.begin(), input.end(), result.residue.begin());

        while (rootMeanSquare(span<const float>(result.residue)) >= 0.01)
        {
            result.intrinsicModeFunctions.emplace_back(findIntrinsicModeFunction(result.residue));
            subtract(span<const float>(result.residue), span<const float>(result.intrinsicModeFunctions.back()), span<float>(result.residue));
        }

        return result;
    }
}
