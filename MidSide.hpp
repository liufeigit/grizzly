/*
 
 This file is a part of Grizzly, a modern C++ library for digital signal
 processing. See https://github.com/dsperados/grizzly for more information.
 
 Copyright (C) 2016 Dsperados <info@dsperados.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 --------------------------------------------------------------------
 
 If you would like to use Grizzly for commercial or closed-source
 purposes, please contact us for a commercial license.
 
 */

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
