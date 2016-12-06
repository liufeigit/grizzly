//
//  ImpulseResponse.hpp
//  grizzly
//
//  Created by Stijn Frishert on 06/12/16.
//
//

#ifndef GRIZZLY_IMPULSE_RESPONSE_HPP
#define GRIZZLY_IMPULSE_RESPONSE_HPP

#include <cstdint>

namespace dsp
{
    // Create an impulse response given a filter and a size
    template <typename Iterator, typename Function>
    void createImpulseResponse(Iterator begin, Iterator end, Function&& filter)
    {
        if (begin == end)
            return;
        
        *begin = filter(1);
        for (auto it = begin + 1; it != end; ++it)
            *it = filter(0);
    }
}

#endif /* GRIZZLY_IMPULSE_RESPONSE_HPP */
