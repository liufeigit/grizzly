//
//  AnalyticTransform.hpp
//  libbear
//
//  Created by Stijn Frishert on 03/02/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#ifndef BEAR_DSP_ANALYTIC_TRANSFORM_HPP
#define BEAR_DSP_ANALYTIC_TRANSFORM_HPP

#include <complex>
#include <vector>

namespace bear::dsp
{
    //! The analytic transform of a real signal
    std::vector<std::complex<float>> analyticTransform(const std::vector<float>& input);
}

#endif /* BEAR_DSP_ANALYTIC_TRANSFORM_HPP */
