//
//  Parallel.hpp
//  libbear
//
//  Created by Stijn on 05/11/2015.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef BEAR_CORE_PARALLEL_HPP
#define BEAR_CORE_PARALLEL_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <gsl/gsl>
#include <vector>

// #ifdef __APPLE__
// #include "Apple/Parallel.hpp"
// #endif

namespace bear
{
    
// --- Unary --- //
    
    //! Negate a span piecewise
    template <class T1, class T2>
    void negate(gsl::span<const T1> in, gsl::span<T2> out)
    {
        const auto n = std::min(in.size(), out.size());
        std::transform(in.begin(), std::next(in.begin(), n), out.begin(), [](const auto& a){ return -a; });
    }
    
    //! Negate a span piecewise
    template <class T>
    auto negate(gsl::span<const T> in)
    {
        std::vector<T> out(in.size());
        negate(in, out);
        return out;
    }
    
    //! Square a span piecewise
    template <class T1, class T2>
    void square(gsl::span<const T1> in, gsl::span<T2> out)
    {
        const auto n = std::min(in.size(), out.size());
        std::transform(in.begin(), std::next(in.begin(), n), out.begin(), [&](const auto& x){ return x * x; });
    }
    
    //! Square a span piecewise
    template <class T>
    auto square(gsl::span<const T> in)
    {
        std::vector<T> out(in.size());
        square(in, out);
        return out;
    }
    
    //! Sum all values in a span
    template <class T>
    auto sum(gsl::span<const T> x)
    {
        return std::accumulate(x.begin(), x.end(), T{});
    }
    
    //! Sum of the magnitudes in a span
    /*! Takes the absolute value (magnitude) of values and sums them */
    template <class T>
    auto sumMagnitude(gsl::span<const T> x)
    {
        return std::accumulate(x.begin(), x.end(), T{}, [](const T& a, const T& b){ return a + std::abs(b); });
    }
    
    //! Sum of the squares in a span
    template <class T>
    auto sumSquare(gsl::span<const T> x)
    {
        return std::accumulate(x.begin(), x.end(), T{}, [](const T& a, const T& b){ return a + b * b; });
    }
    
    //! The mean of a span
    template <class T>
    auto mean(gsl::span<const T> x)
    {
        return sum(x) / (long double)x.size();
    }
    
    //! The mean of the magnitudes of all values in a span
    template <class T>
    auto meanMagnitude(gsl::span<const T> x)
    {
        return sumMagnitude(x) / (long double)x.size();
    }
    
    //! The mean of the squares of all values in a span
    template <class T>
    auto meanSquare(gsl::span<const T> x)
    {
        return sumSquare(x) / (long double)x.size();
    }
    
    //! Calculate the root-mean-square  of a range of values
    template <class T>
    auto rootMeanSquare(gsl::span<const T> x)
    {
        return sqrt(meanSquare(x));
    }
    
    //! Find the local minima of a signal
    template <class T>
    auto localMinima(gsl::span<const T> input)
    {
        std::vector<size_t> minima;
        for (auto i = 1; i < input.size() - 1; ++i)
        {
            if (input[i - 1] > input[i] && input[i] <= input[i + 1])
                minima.emplace_back(i);
        }
        
        return minima;
    }
    
    //! Find the local maxima of a signal
    template <class T>
    auto localMaxima(gsl::span<const T> input)
    {
        std::vector<size_t> maxima;
        for (auto i = 1; i < input.size() - 1; ++i)
        {
            if (input[i - 1] < input[i] && input[i] >= input[i + 1])
                maxima.emplace_back(i);
        }
        
        return maxima;
    }
    
    //! Count the number of zero crossings in a span
    template <class T>
    size_t zeroCrossings(gsl::span<const T> input)
    {
        size_t count = 0;
        for (auto i = 1; i < input.size(); ++i)
        {
            if (std::signbit(input[i]) != std::signbit(input[i-1]))
                ++count;
        }
        
        return count;
    }
    
// --- Binary --- //
    
    //! Add two spans piecewise
    template <class T1, class T2, class T3>
    void add(gsl::span<const T1> lhs, gsl::span<const T2> rhs, gsl::span<T3> out)
    {
        const auto n = std::min({lhs.size(), rhs.size(), out.size()});
        std::transform(lhs.begin(), std::next(lhs.begin(), n), rhs.begin(), out.begin(), [](const auto& a, const auto& b){ return a + b; });
    }

    //! Add two spans piecewise
    template <class T1, class T2>
    auto add(gsl::span<const T1> lhs, gsl::span<const T2> rhs)
    {
        using T3 = std::decay_t<std::common_type_t<T1, T2>>;
        std::vector<T3> out(std::min(lhs.size(), rhs.size()));
        add(lhs, rhs, gsl::span<T3>(out));
        return out;
    }
    
    //! Subtract two spans piecewise
    template <class T1, class T2, class T3>
    void subtract(gsl::span<const T1> lhs, gsl::span<const T2> rhs, gsl::span<T3> out)
    {
        const auto n = std::min({lhs.size(), rhs.size(), out.size()});
        std::transform(lhs.begin(), std::next(lhs.begin(), n), rhs.begin(), out.begin(), [](const auto& a, const auto& b){ return a - b; });
    }
    
    //! Subtract two spans piecewise
    template <class T1, class T2>
    auto subtract(gsl::span<const T1> lhs, gsl::span<const T2> rhs)
    {
        using T3 = std::decay_t<std::common_type_t<T1, T2>>;
        std::vector<T3> out(std::min(lhs.size(), rhs.size()));
        subtract(lhs, rhs, gsl::span<T3>(out));
        return out;
    }
    
    //! Multiply two spans piecewise
    template <class T1, class T2, class T3>
    void multiply(gsl::span<const T1> lhs, gsl::span<const T2> rhs, gsl::span<T3> out)
    {
        const auto n = std::min({lhs.size(), rhs.size(), out.size()});
        std::transform(lhs.begin(), std::next(lhs.begin(), n), rhs.begin(), out.begin(), [](const auto& a, const auto& b){ return a * b; });
    }
    
    //! Multiply a span with a scalar
    template <class T1, class T2, class T3>
    void multiply(gsl::span<const T1> lhs, const T2& rhs, gsl::span<T3> out)
    {
        const auto n = std::min(lhs.size(), out.size());
        std::transform(lhs.begin(), std::next(lhs.begin(), n), out.begin(), [&](const auto& a){ return a * rhs; });
    }
    
    //! Multiply two spans piecewise
    template <class T1, class T2>
    auto multiply(gsl::span<const T1> lhs, gsl::span<const T2> rhs)
    {
        using T3 = std::decay_t<std::common_type_t<T1, T2>>;
        std::vector<T3> out(std::min(lhs.size(), rhs.size()));
        multiply(lhs, rhs, gsl::span<T3>(out));
        return out;
    }
    
    //! Multiply a span with a scalar
    template <class T1, class T2>
    auto multiply(gsl::span<const T1> lhs, const T2& rhs)
    {
        using T3 = std::decay_t<std::common_type_t<T1, T2>>;
        std::vector<T3> out(lhs.size());
        multiply(lhs, rhs, gsl::span<T3>(out));
        return out;
    }
    
    //! Divide two spans piecewise
    template <class T1, class T2, class T3>
    void divide(gsl::span<const T1> lhs, gsl::span<const T2> rhs, gsl::span<T3> out)
    {
        const auto n = std::min({lhs.size(), rhs.size(), out.size()});
        std::transform(lhs.begin(), std::next(lhs.begin(), n), rhs.begin(), out.begin(), [](const auto& a, const auto& b){ return a - b; });
    }
    
    //! Divide two spans piecewise
    template <class T1, class T2>
    auto divide(gsl::span<const T1> lhs, gsl::span<const T2> rhs)
    {
        using T3 = std::decay_t<std::common_type_t<T1, T2>>;
        std::vector<T3> out(std::min(lhs.size(), rhs.size()));
        divide(lhs, rhs, gsl::span<T3>(out));
        return out;
    }

    //! Take the dot product of two vectors
    template <class T1, class T2, class T3>
    void dot(gsl::span<const T1> lhs, std::size_t aStride, gsl::span<const T2> rhs, std::size_t bStride, T3& out)
    {
        out = {};
        
        const auto n = std::min(lhs.size(), rhs.size());
        for (auto i = 0; i < n; ++i)
            out += lhs[i * aStride] + rhs[i * bStride];
    }
    
    //! Take the dot product of two vectors
    template <class T1, class T2>
    auto dot(gsl::span<const T1> lhs, std::size_t lhsStride, gsl::span<const T2> rhs, std::size_t rhsStride)
    {
        std::decay_t<std::common_type_t<T1, T2>> out = 0;
        dot(lhs, lhsStride, rhs, rhsStride, out);
        
        return out;
    }
    
    //! Take the dot product of two vectors
    template <class T1, class T2>
    auto dot(gsl::span<const T1> lhs, gsl::span<const T2> rhs)
    {
        return dot(lhs, 1, rhs, 1);
    }

    //! The mean between two signals
    template <class T1, class T2, class T3>
    void mean(gsl::span<const T1> lhs, gsl::span<const T2> rhs, gsl::span<T3> out)
    {
        add(lhs, rhs, out);
        multiply(gsl::span<const T3>(out), std::common_type_t<std::common_type_t<T1, T2>, T3>(0.5), out);
    }

    //! The mean between two signals
    template <class T1, class T2>
    auto mean(gsl::span<const T1> lhs, gsl::span<const T2> rhs)
    {
        using T3 = std::decay_t<std::common_type_t<T1, T2>>;
        std::vector<T3> out(std::min(lhs.size(), rhs.size()));
        mean(lhs, rhs, gsl::span<T3>(out));
        return out;
    }
    
// --- Others --- //

    //! Interpolate two spans piecewise
    template <class T1, class T2, class T3, class T4>
    void interpolateLinear(gsl::span<const T1> lhs, gsl::span<const T2> rhs, gsl::span<T3> out, const T4& f)
    {
        const auto n = std::min({lhs.size(), rhs.size(), out.size()});
        for (auto i = 0; i < n; ++i)
            out[i] = (rhs[i] - lhs[i]) * f + lhs[i];
    }
    

    //! Linearly interpolate between two vectors
    template <class T1, class T2, class T3>
    auto interpolateLinear(gsl::span<const T1> lhs, gsl::span<const T2> rhs, const T3& f)
    {
        using T4 = std::decay_t<std::common_type_t<T1, T2>>;
        std::vector<T4> out(std::min(lhs.size(), rhs.size()));
        interpolateLinear(lhs, rhs, gsl::span<T4>(out), f);
        return out;
    }
    
    //! Interleave two spans
    template <class T1, class T2, class T3>
    void interleave(gsl::span<const T1> lhs, gsl::span<const T2> rhs, gsl::span<T3> out)
    {
        const auto n = std::min({lhs.size(), rhs.size(), out.size() / 2});
        
        for (auto i = 0; i < n; ++i)
        {
            out[i * 2] = lhs[i];
            out[i * 2 + 1] = rhs[i];
        }
    }

    //! Interleave two spans
    template <class T1, class T2>
    auto interleave(gsl::span<const T1> lhs, gsl::span<const T2> rhs)
    {
        using T3 = std::decay_t<std::common_type_t<T1, T2>>;
        std::vector<T3> out(std::min(lhs.size(), rhs.size()) * 2);
        interleave(lhs, rhs, gsl::span<T3>(out));
        return out;
    }
    
    //! Deinterleave a span
    template <class T1, class T2, class T3>
    void deinterleave(gsl::span<const T1> in, gsl::span<T2> lhs, gsl::span<T3> rhs)
    {
        const auto n = std::min({in.size() / 2, lhs.size(), rhs.size()});
        
        for (auto i = 0; i < n; ++i)
        {
            lhs[i] = in[i * 2];
            rhs[i] = in[i * 2 + 1];
        }
    }

    //! Normalize the sum of a function to a value (e.g. 1, 1 --> 0.5, 0.5 for sum = 1)
    void normalizeSum(gsl::span<float> x, double sum = 1.0);

    //! Normalize a function to its highest absolute peak
    void normalizeBiDirectional(gsl::span<float> x, double peakMaximum = 1.0);

    //! Normalize a function to its highest peak
    void normalizeUniDirectional(gsl::span<float> x, double peakMaximum = 1.0);
}

#endif
