//
//  FastFourierTransformBase.cpp
//  bear-dsp
//
//  Created by Stijn on 10/06/2016.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#include <cassert>

#include "FastFourierTransformBase.hpp"

using namespace math;
using namespace std;

namespace dsp
{
    FastFourierTransformBase::FastFourierTransformBase(size_t size) :
        size(size)
    {
        
    }
    
    vector<complex<float>> FastFourierTransformBase::forward(const float* input)
    {
        vector<complex<float>> output(size / 2 + 1);
        forward(input, output.begin());
        return output;
    }
    
    vector<complex<double>> FastFourierTransformBase::forward(const double* input)
    {
        vector<complex<double>> output(size / 2 + 1);
        forward(input, output.begin());
        return output;
    }
}
