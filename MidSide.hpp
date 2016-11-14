//
//  MidSide.hpp
//  libbear
//
//  Created by Milan van der Meer on 07/03/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#ifndef GRIZZLY_MID_SIDE_HPP
#define GRIZZLY_MID_SIDE_HPP

#include <utility>
#include <vector>

namespace dsp
{
    //! Sample with a left and right channel
    template <class T>
    struct Stereo
    {
    public:
        Stereo() = default;
        Stereo(const T& left, const T& right) : left(left), right(right) { }

        T left; //!< The left channel
        T right; //!< The right channel
    };

    //! Compare two stereos for equality
    template <class T>
    constexpr bool operator==(const Stereo<T>& lhs, const Stereo<T>& rhs)
    {
        return lhs.left == rhs.left && lhs.right == rhs.right;
    }

    //! Compare two stereos for inequality
    template <class T>
    constexpr bool operator!=(const Stereo<T>& lhs, const Stereo<T>& rhs)
    {
        return !(lhs == rhs);
    }

    //! Sample with a mid and side channel
    template <class T>
    struct MidSide
    {
        MidSide() = default;
        MidSide(const T& mid, const T& side) : mid(mid), side(side) { }

        T mid; //!< The mid channel
        T side; //!< The side channel
    };

    //! Compare two mid-sides for equality
    template <class T>
    constexpr bool operator==(const MidSide<T>& lhs, const MidSide<T>& rhs)
    {
        return lhs.mid == rhs.mid && lhs.side == rhs.side;
    }

    //! Compare two mid-sides for inequality
    template <class T>
    constexpr bool operator!=(const MidSide<T>& lhs, const MidSide<T>& rhs)
    {
        return !(lhs == rhs);
    }

    //! Convert a left and right stereo sample to mid-side
    template <class T>
    constexpr MidSide<T> stereo2ms(const T& left, const T& right)
    {
        return {(left + right) * static_cast<T>(0.5), (left - right) * static_cast<T>(0.5)};
    }

    //! Convert a left and right stereo sample to mid-side
    template <class T>
    constexpr MidSide<T> stereo2ms(const Stereo<T>& stereo)
    {
        return stereo2ms(stereo.left, stereo.right);
    }

    //! Convert mid-side sample to a stereo left-right
    template <class T>
    constexpr Stereo<T> ms2stereo(const T& mid, const T& side)
    {
        return {mid + side, mid - side};
    }

    //! Convert mid-side sample to a stereo left-right
    template <class T>
    constexpr Stereo<T> ms2stereo(const MidSide<T>& ms)
    {
        return ms2stereo(ms.mid, ms.side);
    }
}

#endif
