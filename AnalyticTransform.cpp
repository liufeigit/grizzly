//
//  AnalyticTransform.cpp
//  libbear
//
//  Created by Stijn Frishert on 03/02/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#include <cassert>

#include "AnalyticTransform.hpp"
#include "HilbertTransform.hpp"

using namespace gsl;
using namespace std;

namespace bear::dsp
{
    vector<std::complex<float>> analyticTransform(const vector<float>& input)
    {
        auto hilbert = hilbertTransform(input);

        assert(hilbert.size() == input.size());

        vector<std::complex<float>> analyticSignal(input.size());
        for (auto i = 0; i < input.size(); ++i)
        {
            analyticSignal[i].real(input[i]);
            analyticSignal[i].imag(hilbert[i]);
        }

        return analyticSignal;
    }
}
