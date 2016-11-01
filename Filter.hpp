//
//  Filter.hpp
//  bear-dsp
//
//  Created by Stijn Frishert on 7/13/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef BEAR_DSP_FILTER_HPP
#define BEAR_DSP_FILTER_HPP

namespace bear::dsp
{
    //! A filter interface: takes an input sample, produces an output sample
    template <class In, class Out = In>
    class Filter
    {
    public:
        //! Virtual destructor
        virtual ~Filter() = default;
        
        //! Take a sample and produce another
        Out operator()(const In& x) { return process(x); }
        
        //! Take a sample and produce another
        virtual Out process(const In& x) = 0;
    };
}

#endif /* BEAR_DSP_FILTER_HPP */
