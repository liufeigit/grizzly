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
    
    inline static auto sum(const std::vector<float>& in)
    {
        float out;
        vDSP_sve(in.data(), 1, &out, in.size());
        return out;
    }
    
    inline static auto sum(const std::vector<double>& in)
    {
        double out;
        vDSP_sveD(in.data(), 1, &out, in.size());
        return out;
    }
    
    inline static auto sumMagnitude(const std::vector<float>& in)
    {
        float out;
        vDSP_svemg(in.data(), 1, &out, in.size());
        return out;
    }
    
    inline static auto sumMagnitude(const std::vector<double>& in)
    {
        double out;
        vDSP_svemgD(in.data(), 1, &out, in.size());
        return out;
    }
    
    inline static auto sumSquare(const std::vector<float>& in)
    {
        float out;
        vDSP_svesq(in.data(), 1, &out, in.size());
        return out;
    }
    
    inline static auto sumSquare(const std::vector<double>& in)
    {
        double out;
        vDSP_svesqD(in.data(), 1, &out, in.size());
        return out;
    }
    
    auto mean(const std::vector<float>& in)
    {
        float out;
        vDSP_meanv(in.data(), 1, &out, in.size());
        return out;
    }
    
    auto mean(const std::vector<double>& in)
    {
        double out;
        vDSP_meanvD(in.data(), 1, &out, in.size());
        return out;
    }
    
    auto meanMagnitude(const std::vector<float>& in)
    {
        float out;
        vDSP_meamgv(in.data(), 1, &out, in.size());
        return out;
    }
    
    auto meanMagnitude(const std::vector<double>& in)
    {
        double out;
        vDSP_meamgvD(in.data(), 1, &out, in.size());
        return out;
    }
    
    auto meanSquare(const std::vector<float>& in)
    {
        float out;
        vDSP_measqv(in.data(), 1, &out, in.size());
        return out;
    }
    
    auto meanSquare(const std::vector<double>& in)
    {
        double out;
        vDSP_measqvD(in.data(), 1, &out, in.size());
        return out;
    }
    
    auto rootMeanSquare(const std::vector<float>& in)
    {
        float out;
        vDSP_rmsqv(in.data(), 1, &out, in.size());
        return out;
    }
    
    auto rootMeanSquare(const std::vector<double>& in)
    {
        double out;
        vDSP_rmsqvD(in.data(), 1, &out, in.size());
        return out;
    }
    
    size_t zeroCrossings(const std::vector<float>& in)
    {
        vDSP_Length last = 0;
        vDSP_Length count = 0;
        vDSP_nzcros(in.data(), 1, N, &last, &count, in.size());
        
        return count;
    }
    
    size_t zeroCrossings(const std::vector<double>& in)
    {
        vDSP_Length last = 0;
        vDSP_Length count = 0;
        vDSP_nzcrosD(in.data(), 1, N, &last, &count, in.size());
        
        return count;
    }
    
// --- Binary --- //
    
    template <long N1, long N2>
    inline static void negate(const std::vector<float>& in, std::vector<float>& out)
    {
        vDSP_vneg(in.data(), 1, out.data(), 1, std::min(in.size(), out.size()));
    }
    
    template <long N1, long N2>
    inline static void negate(const std::vector<double>& in, std::vector<double>& out)
    {
        vDSP_vnegD(in.data(), 1, out.data(), 1, std::min(in.size(), out.size()));
    }
    
    template <long N1, long N2>
    inline static void square(const std::vector<float>& in, std::vector<float>& out)
    {
        vDSP_vsq(in.data(), 1, out.data(), 1, std::min(in.size(), out.size()));
    }
    
    template <long N1, long N2>
    inline static void square(const std::vector<double>& in, std::vector<double>& out)
    {
        vDSP_vsqD(in.data(), 1, out.data(), 1, std::min(in.size(), out.size()));
    }
    
// --- Tertiary --- //
    
    inline static void add(const std::vector<float>& lhs, const std::vector<float>& rhs, std::vector<float>& out)
    {
        vDSP_vadd(lhs.data(), 1, rhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    inline static void add(const std::vector<double>& lhs, const std::vector<double>& rhs, std::vector<double>& out)
    {
        vDSP_vaddD(lhs.data(), 1, rhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    inline static void subtract(const std::vector<float>& lhs, const std::vector<float>& rhs, std::vector<float>& out)
    {
        vDSP_vsub(rhs.data(), 1, lhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    inline static void subtract(const std::vector<double>& lhs, const std::vector<double>& rhs, std::vector<double>& out)
    {
        vDSP_vsubD(rhs.data(), 1, lhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    inline static void multiply(const std::vector<float>& lhs, const std::vector<float>& rhs, std::vector<float>& out)
    {
        vDSP_vmul(lhs.data(), 1, rhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    inline static void multiply(const std::vector<double>& lhs, const std::vector<double>& rhs, std::vector<double>& out)
    {
        vDSP_vmulD(lhs.data(), 1, rhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    inline static void multiply(const std::vector<float>& lhs, const float rhs, std::vector<float>& out)
    {
        vDSP_vsmul(lhs.data(), 1, &rhs, out.data(), 1, std::min(lhs.size(), out.size()));
    }
    
    inline static void multiply(const std::vector<double>& lhs, const double rhs, std::vector<double>& out)
    {
        vDSP_vsmulD(lhs.data(), 1, &rhs, out.data(), 1, std::min(lhs.size(), out.size()));
    }
    
    inline static void divide(const std::vector<float>& lhs, const std::vector<float>& rhs, std::vector<float>& out)
    {
        vDSP_vdiv(rhs.data(), 1, lhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    inline static void divide(const std::vector<double>& lhs, const std::vector<double>& rhs, std::vector<double>& out)
    {
        vDSP_vdivD(rhs.data(), 1, lhs.data(), 1, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }

    inline static void dot(const std::vector<float>& lhs, size_t lhsStride, const std::vector<float>& rhs, size_t rhsStride, float& out)
    {
        vDSP_dotpr(lhs.data(), lhsStride, rhs.data(), rhsStride, &out, std::min(lhs.size(), rhs.size()));
    }
    
    inline static void dot(const std::vector<double>& lhs, size_t lhsStride, const std::vector<double>& rhs, size_t rhsStride, float& out)
    {
        vDSP_dotprD(lhs.data(), lhsStride, rhs.data(), rhsStride, &out, std::min(lhs.size(), rhs.size()));
    }
    
// --- Others --- //
    
    inline static void interpolateLinear(const std::vector<float>& lhs, const std::vector<float>& rhs, std::vector<float>& out, const float& f)
    {
        vDSP_vintb(lhs.data(), 1, rhs.data(), 1, &f, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    inline static void interpolateLinear(const std::vector<double>& lhs, const std::vector<double>& rhs, std::vector<double>& out, const double& f)
    {
        vDSP_vintbD(lhs.data(), 1, rhs.data(), 1, &f, out.data(), 1, std::min({lhs.size(), rhs.size(), out.size()}));
    }
    
    inline static void interleave(const std::vector<float>& lhs, const std::vector<float>& rhs, std::vector<float>& out)
    {
        DSPSplitComplex split;
        split.realp = const_cast<float*>(lhs.data());
        split.imagp = const_cast<float*>(rhs.data());
        
        vDSP_ztoc(&split, 1, (DSPComplex*)out.data(), 2, std::min({lhs.size(), rhs.size(), out.size() / 2}));
    }
    
    inline static void interleave(const std::vector<double>& lhs, const std::vector<double>& rhs, std::vector<double>& out)
    {
        DSPDoubleSplitComplex split;
        split.realp = const_cast<double*>(lhs.data());
        split.imagp = const_cast<double*>(rhs.data());
        
        vDSP_ztocD(&split, 1, (DSPDoubleComplex*)out.data(), 2, std::min({lhs.size(), rhs.size(), out.size() / 2}));
    }
    
    inline static void deinterleave(const std::vector<float>& in, const std::vector<float>& lhs, std::vector<float>& rhs)
    {
        DSPSplitComplex split;
        split.realp = lhs.data();
        split.imagp = rhs.data();
        
        vDSP_ctoz((DSPComplex*)in.data(), 2, &split, 1, std::min({in.size() / 2, lhs.size(), rhs.size()}));
    }
    
    inline static void deinterleave(const std::vector<double>& in, const std::vector<double>& lhs, std::vector<double>& rhs)
    {
        DSPDoubleSplitComplex split;
        split.realp = lhs.data();
        split.imagp = rhs.data();
        
        vDSP_ctozD((DSPDoubleComplex*)in.data(), 2, &split, 1, std::min({in.size() / 2, lhs.size(), rhs.size()}));
    }
}

#endif
