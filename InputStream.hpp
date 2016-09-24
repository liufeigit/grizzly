//
//  InputStream.hpp
//  bear-dsp
//
//  Created by Stijn Frishert on 01/06/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef BEAR_DSP_INPUT_STREAM_HPP
#define BEAR_DSP_INPUT_STREAM_HPP

#include <boost/circular_buffer.hpp>
#include <unit/discrete.hpp>
#include <stdexcept>

#include "Access.hpp"
#include "Interpolation.hpp"

namespace bear::dsp
{
    //! Base class for input reading streams
    template <class T>
    class InputStream
    {
    public:
        virtual ~InputStream() = default;

        //! Read the next element
        /*! Throws an exception when the stream is exhausted */
        inline T read()
        {
            T x;
            read(x);

            return x;
        }

        //! Read the next element
        /*! Throws an exception when the stream is exhausted */
        inline void read(T& x)
        {
            // If the stream is exhausted, we shouldn't read
            if (exhausted())
                throw std::runtime_error("InputStream is exhausted");

            // Retrieve the next sample
            next(x);

            // Let others know the stream was exhausted
            if (exhausted() && onExhaustion)
                onExhaustion();
        }

        //! Is the stream exhausted?
        virtual bool exhausted() const = 0;

    public:
        //! Called when the stream gets exhausted
        std::function<void()> onExhaustion;

    private:
        //! Derivatives should implement this function for retrieving new elements
        virtual void next(T& x) = 0;
    };

    //! Interpolates an InputStream
    template <class T>
    class InputStreamInterpolator
    {
    public:
        //! Construct the interpolator
        InputStreamInterpolator(InputStream<T>& stream) :
            stream(stream)
        {

        }

        //! Give a sample and increase the stream by a given index increase
        template <class Interpolator = LinearInterpolation>
        T increase(unit::discrete<double> increase, Interpolator interpolator = Interpolator())
        {
            // If we're exhausted, we can't generate any more samples
            if (exhausted())
                throw std::runtime_error("StreamInterpolator exhausted");

            // Make sure we have enough samples in the buffer
            resizeBuffer(Interpolator::size);

            // Generate the output sample
            auto y = interpolate(buffer.begin(), buffer.end(), index, interpolator, ClampedAccess());

            // Increase the index
            index += increase;

            // Compute the turning point index, at which all samples are shifted and a new one is added (if available)
            const std::size_t threshold = (buffer.size() + 1) / 2;

            // Have we reached the turning point?
            while (index >= threshold)
            {
                --index;
                next();
            }

            return y;
        }

        //! Is the stream (and interpolation) exhausted?
        inline bool exhausted() const
        {
            if (!stream.exhausted())
                return false;

            return buffer.size() == 0 || (buffer.size() == 1 && index != 0);
        }

    private:
        //! Make sure the buffer has the correct size
        void resizeBuffer(std::size_t size)
        {
            if (buffer.capacity() == size)
                return;

            buffer.rset_capacity(size);
            while (buffer.reserve() && !stream.exhausted())
                next();
        }

        //! Either put a new sample in the buffer, or remove past if the stream is exhausted
        void next()
        {
            if (stream.exhausted())
                buffer.rresize(buffer.size() - 1);
            else
                buffer.push_back(stream.read());
        }

    private:
        //! The stream used as input
        InputStream<T>& stream;

        //! The current index in the buffer
        unit::discrete<double> index = 0;

        //! A circular buffer of samples from the stream
        boost::circular_buffer<T> buffer;
    };

}

#endif
