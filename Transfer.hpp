//
//  Transfer.hpp
//  bear-core
//
//  Created by Milan van der Meer on 19/07/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef BEAR_CORE_TRANSFER_HPP
#define BEAR_CORE_TRANSFER_HPP

#include <functional>
#include <vector>

#include "Math.hpp"
#include "Range.hpp"

namespace bear
{
    //! Generate a curve from a range
    /*! @param range Range for transfer
        @param resolution The number of entry points (equally placed) in range, determines output size
        @param transferFunction A unary function which transforms x -> y for each entry point */
    template <class T1, class T2>
    std::vector<T2> generateCurve(const T1 minimum, const T2 maximum, std::size_t resolution, std::function<T2(T2)> transferFunction)
    {
        const auto increment = static_cast<long double>(maximum - minimum) / resolution;
        
        std::vector<T2> y;
        y.reserve(resolution);
        
        for (auto i = 0; i < resolution; ++i)
            y.emplace_back(transferFunction((i * increment) + minimum));
        
        return y;
    }
}

#endif
