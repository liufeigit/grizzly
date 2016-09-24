//
//  ParallelAccelerate.hpp
//  bear-core
//
//  Created by Stijn Frishert on 08/06/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef BEAR_CORE_PARALLEL_ACCELERATE_HPP
#define BEAR_CORE_PARALLEL_ACCELERATE_HPP

#include <Accelerate/Accelerate.h>

namespace bear
{
    
// --- Unary --- //
    
    template <long N>
    inline static auto sum(gsl::span<const float, N> in)
    {
        float out;
        vDSP_sve(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    inline static auto sum(gsl::span<const double, N> in)
    {
        double out;
        vDSP_sveD(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    inline static auto sumMagnitude(gsl::span<const float, N> in)
    {
        float out;
        vDSP_svemg(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    inline static auto sumMagnitude(gsl::span<const double, N> in)
    {
        double out;
        vDSP_svemgD(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    inline static auto sumSquare(gsl::span<const float, N> in)
    {
        float out;
        vDSP_svesq(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    inline static auto sumSquare(gsl::span<const double, N> in)
    {
        double out;
        vDSP_svesqD(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    auto mean(gsl::span<const float, N> in)
    {
        float out;
        vDSP_meanv(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    auto mean(gsl::span<const double, N> in)
    {
        double out;
        vDSP_meanvD(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    auto meanMagnitude(gsl::span<const float, N> in)
    {
        float out;
        vDSP_meamgv(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    auto meanMagnitude(gsl::span<const double, N> in)
    {
        double out;
        vDSP_meamgvD(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    auto meanSquare(gsl::span<const float, N> in)
    {
        float out;
        vDSP_measqv(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    auto meanSquare(gsl::span<const double, N> in)
    {
        double out;
        vDSP_measqvD(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    auto rootMeanSquare(gsl::span<const float, N> in)
    {
        float out;
        vDSP_rmsqv(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    auto rootMeanSquare(gsl::span<const double, N> in)
    {
        double out;
        vDSP_rmsqvD(in.data(), 1, &out, in.size());
        return out;
    }
    
    template <long N>
    size_t zeroCrossings(gsl::span<const float, N> in)
    {
        vDSP_Length last = 0;
        vDSP_Length count = 0;
        vDSP_nzcros(in.data(), 1, N, &last, &count, in.size());
        
        return count;
    }
    
    template <long N>
    size_t zeroCrossings(gsl::span<const double, N> in)
    {
        vDSP_Length last = 0;
        vDSP_Length count = 0;
        vDSP_nzcrosD(in.data(), 1, N, &last, &count, in.size());
        
        return count;
    }
    
// --- Binary --- //
    
    template <long N1, long N2>
    inline static void negate(gsl::span<const float, N1> in, gsl::span<float, N2> out)
    {
        vDSP_vneg(in.data(), 1, out.data(), 1, std::min(in.size(), out.size()));
    }
    
    template <long N1, long N2>
    inline static void negate(gsl::span<const double, N1> in, gsl::span<double, N2> out)
    {
        vDSP_vnegD(in.data(), 1, out.data(), 1, std::min(in.size(), out.size()));
    }
    
    template <long N1, long N2>
    inline static void square(gsl::span<const float, N1> in, gsl::span<float, N2> out)
    {
        vDSP_vsq(in.data(), 1, out.data(), 1, std::min(in.size(), out.size()));
    }
    
    template <long N1, long N2>
    inline static void square(gsl::span<const double, N1> in, gsl::span<double, N2> out)
    {
        vDSP_vsqD(in.data(), 1, out.data(), 1, std::min(in.size(), out.size()));
    }
    
// --- Tertiary --- //
    
    template <long N1, long N2, long N3>
    inline static void add(gsl::span<const float, N1> lhs, gsl::span<const float, N2> rhs, gsl::span<float, N3> out)
    {
        vDSP_vadd(lhs.data(), 1, rhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    template <long N1, long N2, long N3>
    inline static void add(gsl::span<const double, N1> lhs, gsl::span<const double, N2> rhs, gsl::span<double, N3> out)
    {
        vDSP_vaddD(lhs.data(), 1, rhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    template <long N1, long N2, long N3>
    inline static void subtract(gsl::span<const float, N1> lhs, gsl::span<const float, N2> rhs, gsl::span<float, N3> out)
    {
        vDSP_vsub(rhs.data(), 1, lhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    template <long N1, long N2, long N3>
    inline static void subtract(gsl::span<const double, N1> lhs, gsl::span<const double, N2> rhs, gsl::span<double, N3> out)
    {
        vDSP_vsubD(rhs.data(), 1, lhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    template <long N1, long N2, long N3>
    inline static void multiply(gsl::span<const float, N1> lhs, gsl::span<const float, N2> rhs, gsl::span<float, N3> out)
    {
        vDSP_vmul(lhs.data(), 1, rhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    template <long N1, long N2, long N3>
    inline static void multiply(gsl::span<const double, N1> lhs, gsl::span<const double, N2> rhs, gsl::span<double, N3> out)
    {
        vDSP_vmulD(lhs.data(), 1, rhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    template <long N1, long N3>
    inline static void multiply(gsl::span<const float, N1> lhs, const float rhs, gsl::span<float, N3> out)
    {
        vDSP_vsmul(lhs.data(), 1, &rhs, out.data(), 1, std::min(lhs.size(), out.size()));
    }
    
    template <long N1, long N3>
    inline static void multiply(gsl::span<const double, N1> lhs, const double rhs, gsl::span<double, N3> out)
    {
        vDSP_vsmulD(lhs.data(), 1, &rhs, out.data(), 1, std::min(lhs.size(), out.size()));
    }
    
    template <long N1, long N2, long N3>
    inline static void divide(gsl::span<const float, N1> lhs, gsl::span<const float, N2> rhs, gsl::span<float, N3> out)
    {
        vDSP_vdiv(rhs.data(), 1, lhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    template <long N1, long N2, long N3>
    inline static void divide(gsl::span<const double, N1> lhs, gsl::span<const double, N2> rhs, gsl::span<double, N3> out)
    {
        vDSP_vdivD(rhs.data(), 1, lhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }

    template <long N1, long N2>
    inline static void dot(gsl::span<const float, N1> lhs, size_t lhsStride, gsl::span<const float, N2> rhs, size_t rhsStride, float& out)
    {
        vDSP_dotpr(lhs.data(), lhsStride, rhs.data(), rhsStride, &out, std::min(lhs.size(), rhs.size()));
    }
    
    template <long N1, long N2>
    inline static void dot(gsl::span<const double, N1> lhs, size_t lhsStride, gsl::span<const double, N2> rhs, size_t rhsStride, float& out)
    {
        vDSP_dotprD(lhs.data(), lhsStride, rhs.data(), rhsStride, &out, std::min(lhs.size(), rhs.size()));
    }
    
// --- Others --- //
    
    template <long N1, long N2, long N3>
    inline static void interpolateLinear(gsl::span<const float, N1> lhs, gsl::span<const float, N2> rhs, gsl::span<float, N3> out, const float& f)
    {
        vDSP_vintb(lhs.data(), 1, rhs.data(), 1, &f, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    template <long N1, long N2, long N3>
    inline static void interpolateLinear(gsl::span<const double, N1> lhs, gsl::span<const double, N2> rhs, gsl::span<double, N3> out, const double& f)
    {
        vDSP_vintbD(lhs.data(), 1, rhs.data(), 1, &f, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    template <long N1, long N2, long N3>
    inline static void interleave(gsl::span<const float, N1> lhs, gsl::span<const float, N2> rhs, gsl::span<float, N3> out)
    {
        DSPSplitComplex split;
        split.realp = const_cast<float*>(lhs.data());
        split.imagp = const_cast<float*>(rhs.data());
        
        vDSP_ztoc(&split, 1, (DSPComplex*)out.data(), 2, std::min({lhs.size(), rhs.size(), out.size() / 2}));
    }
    
    template <long N1, long N2, long N3>
    inline static void interleave(gsl::span<const double, N1> lhs, gsl::span<const double, N2> rhs, gsl::span<double, N3> out)
    {
        DSPDoubleSplitComplex split;
        split.realp = const_cast<double*>(lhs.data());
        split.imagp = const_cast<double*>(rhs.data());
        
        vDSP_ztocD(&split, 1, (DSPDoubleComplex*)out.data(), 2, std::min({lhs.size(), rhs.size(), out.size() / 2}));
    }
    
    template <long N1, long N2, long N3>
    inline static void deinterleave(gsl::span<const float, N1> in, gsl::span<const float, N2> lhs, gsl::span<float, N3> rhs)
    {
        DSPSplitComplex split;
        split.realp = lhs.data();
        split.imagp = rhs.data();
        
        vDSP_ctoz((DSPComplex*)in.data(), 2, &split, 1, std::min({in.size() / 2, lhs.size(), rhs.size()}));
    }
    
    template <long N1, long N2, long N3>
    inline static void deinterleave(gsl::span<const double, N1> in, gsl::span<const double, N2> lhs, gsl::span<double, N3> rhs)
    {
        DSPDoubleSplitComplex split;
        split.realp = lhs.data();
        split.imagp = rhs.data();
        
        vDSP_ctozD((DSPDoubleComplex*)in.data(), 2, &split, 1, std::min({in.size() / 2, lhs.size(), rhs.size()}));
    }
}

#endif
