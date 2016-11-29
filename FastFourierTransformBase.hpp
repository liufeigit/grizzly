//
//  FastFourierTransformBase.hpp
//  bear-dsp
//
//  Created by Stijn on 10/06/2016.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef FAST_FOURIER_TRANSFORM_BASE_HPP
#define FAST_FOURIER_TRANSFORM_BASE_HPP

#include <cstddef>
#include <complex>
#include <dsperados/math/interleave.hpp>
#include <vector>

namespace dsp
{
    //! Base class for Fourier transforms
    class FastFourierTransformBase
    {
    public:
        //! Construct by passing the size
        FastFourierTransformBase(std::size_t size);
        
        //! Virtual destructor, because this is a polymorphic base class
        virtual ~FastFourierTransformBase() = default;
        
    // --- Real --- //
        
        //! Do the forward Fourier transform
        /*! @param input: Address of the input data, containing at least size elements
            @return: A spectrum of (size / 2 + 1) complex bins */
        std::vector<std::complex<float>> forward(const float* input);
        
        //! Do the forward Fourier transform
        /*! @param input: Address of the input data, containing at least size elements
            @return: A spectrum of (size / 2 + 1) complex bins */
        std::vector<std::complex<double>> forward(const double* input);
        
        //! Do the forward Fourier transform
        /*! @param input: Address of the input data, containing at least size elements
            @param output: Iterator to a std::complex<float> spectrum container, of at least (size / 2 + 1) elements */
        template <class ComplexIterator>
        void forward(const float* input, ComplexIterator output);
        
        //! Do the forward Fourier transform
        /*! @param input: Address of the input data, containing at least size elements
            @param output: Iterator to a std::complex<double> spectrum container, of at least (size / 2 + 1) elements */
        template <class ComplexIterator>
        void forward(const double* input, ComplexIterator output);
        
        //! Do the forward Fourier transform
        /*! @param input: Address of the input data, containing at least size elements
            @param real: Address of the real part of the output spectrum, containing at least (size / 2 + 1) elements
            @param imaginary: Address of the imaginary part of the output spectrum, containing at least (size / 2 + 1) elements */
        virtual void forward(const float* input, float* real, float* imaginary) = 0;
        
        //! Do the forward Fourier transform
        /*! @param input: Address of the input data, containing at least size elements
            @param real: Address of the real part of the output spectrum, containing at least (size / 2 + 1) elements
            @param imaginary: Address of the imaginary part of the output spectrum, containing at least (size / 2 + 1) elements */
        virtual void forward(const double* input, double* real, double* imaginary) = 0;
        
        //! Do the inverse Fourier transform
        /* @param input: Iterator to a std::complex<float> or std::complex<double> spectrum container, of at least (size / 2 + 1) elements
            @return: A spectrum signal of (size / 2 + 1) elements */
        template <class ComplexIterator>
        std::vector<typename ComplexIterator::value_type::value_type> inverse(ComplexIterator input);
        
        //! Do the inverse Fourier transform
        /*! @param input: Iterator to a std::complex<float> spectrum container, of at least (size / 2 + 1) elements 
            @param output: Address of the output data, containing at least size elements */
        template <class ComplexIterator>
        void inverse(ComplexIterator input, float* output);
        
        //! Do the inverse Fourier transform
        /*! @param input: Iterator to a std::complex<double> spectrum container, of at least (size / 2 + 1) elements
            @param output: Address of the output data, containing at least size elements */
        template <class ComplexIterator>
        void inverse(ComplexIterator input, double* output);
        
        //! Do the inverse Fourier transform
        /*! @param real: Address of the real part of the input spectrum, containing at least (size / 2 + 1) elements
            @param imaginary: Address of the imaginary part of the input spectrum, containing at least (size / 2 + 1) elements
            @param output: Address of the output data, containing at least size elements */
        virtual void inverse(const float* real, const float* imaginary, float* output) = 0;
        
        //! Do the inverse Fourier transform
        /*! @param real: Address of the real part of the input spectrum, containing at least (size / 2 + 1) elements
            @param imaginary: Address of the imaginary part of the input spectrum, containing at least (size / 2 + 1) elements
            @param output: Address of the output data, containing at least size elements */
        virtual void inverse(const double* real, const double* imaginary, double* output) = 0;
        
    // --- Complex --- //
        
        //! Do the complex forward Fourier transform
        /*! @param input: Iterator to a std::complex<float> or std::complex<double> container, of at least size elements
            @return: A spectrum signal of size elements */
        template <class ComplexIterator>
        std::vector<std::complex<typename ComplexIterator::value_type::value_type>> forwardComplex(ComplexIterator input);
        
        //! Do the complex forward Fourier transform
        /*! @param input: Iterator to a std::complex<float> or std::complex<double> container, of at least size elements
            @param output: Iterator to a std::complex<float> or std::complex<double> spectrum container, of at least size elements
            @note: The iterators should either point both to std::complex<float> containers, or std::complex<double> */
        template <class ComplexInputIterator, class ComplexOutputIterator>
        void forwardComplex(ComplexInputIterator input, ComplexOutputIterator output);
        
        //! Do the complex forward Fourier transform
        /*! @param inReal: Address of the real part of the input, containing at least size elements
            @param inImaginary: Address of the real part of input, containing at least size elements
            @param inReal: Address of the real part of the output spectrum, containing at least size elements
            @param inImaginary: Address of the real part of output spectrum, containing at least size elements */
        virtual void forwardComplex(const float* inReal, const float* inImaginary, float* outReal, float* outImaginary) = 0;
        
        //! Do the complex forward Fourier transform
        /*! @param inReal: Address of the real part of the input, containing at least size elements
            @param inImaginary: Address of the real part of input, containing at least size elements
            @param inReal: Address of the real part of the output spectrum, containing at least size elements
            @param inImaginary: Address of the real part of output spectrum, containing at least size elements */
        virtual void forwardComplex(const double* inReal, const double* inImaginary, double* outReal, double* outImaginary) = 0;
        
        //! Do the complex inverse Fourier transform
        /*! @param input: Iterator to a std::complex<float> or std::complex<double> spectrum container, of at least size elements
            @return: A "time-domain" signal of size elements */
        template <class ComplexIterator>
        std::vector<std::complex<typename ComplexIterator::value_type::value_type>> inverseComplex(ComplexIterator input);
        
        //! Do the complex inverse forward Fourier transform
        /*! @param input: Iterator to a std::complex<float> or std::complex<double> spectrum container, of at least size elements
            @param output: Iterator to a std::complex<float> or std::complex<double> container, of at least size elements
            @note: The iterators should either point both to std::complex<float> containers, or std::complex<double> */
        template <class ComplexInputIterator, class ComplexOutputIterator>
        void inverseComplex(ComplexInputIterator input, ComplexOutputIterator output);
        
        //! Do the complex inverse Fourier transform
        /*! @param inReal: Address of the real part of the input spectrum, containing at least size elements
            @param inImaginary: Address of the real part of input spectrum, containing at least size elements
            @param inReal: Address of the real part of the output, containing at least size elements
            @param inImaginary: Address of the real part of output, containing at least size elements */
        virtual void inverseComplex(const float* inReal, const float* inImaginary, float* outReal, float* outImaginary) = 0;
        
        //! Do the complex inverse Fourier transform
        /*! @param inReal: Address of the real part of the input spectrum, containing at least size elements
            @param inImaginary: Address of the real part of input spectrum, containing at least size elements
            @param inReal: Address of the real part of the output, containing at least size elements
            @param inImaginary: Address of the real part of output, containing at least size elements */
        virtual void inverseComplex(const double* inReal, const double* inImaginary, double* outReal, double* outImaginary) = 0;
        
        //! Return the size this FFT operates with (= equal to the size of the input)
        std::size_t getSize() const { return size; }
        
    protected:
        //! The frame size
        std::size_t size = 0;
    };
    
    template <class InputIterator1, class InputIterator2, class ComplexOutputIterator>
    void interleave(InputIterator1 inBegin1, InputIterator1 inEnd, InputIterator2 rhs, ComplexOutputIterator outBegin)
    {
        std::vector<typename ComplexOutputIterator::value_type::value_type> fout(std::distance(inBegin1, inEnd) * 2);
        math::interleave(inBegin1, inEnd, rhs, fout.begin());
        
        for (auto it = fout.begin(); it != fout.end(); ++it)
        {
            outBegin->real(*it);
            outBegin->imag(*(++it));
            
            ++outBegin;
        }
    }
    
    template <class ComplexInputIterator, class OutputIterator1, class OutputIterator2>
    void deinterleave(ComplexInputIterator inBegin, ComplexInputIterator inEnd, OutputIterator1 outBegin1, OutputIterator2 outBegin2)
    {
        std::vector<typename ComplexInputIterator::value_type::value_type> fin(std::distance(inBegin, inEnd) * 2);
        
        for (auto it = fin.begin(); it != fin.end(); ++it)
        {
            *it = inBegin->real();
            *(++it) = inBegin->imag();
            
            ++inBegin;
        }
        
        math::deinterleave(fin.begin(), fin.end(), outBegin1, outBegin2);
    }
    
    template <class ComplexIterator>
    void FastFourierTransformBase::forward(const float* input, ComplexIterator output)
    {
        static_assert(std::is_same<typename ComplexIterator::value_type, std::complex<float>>::value, "should be an iterator to std::complex<float>");
        
        // The deinterleaved output will be stored in here
        std::vector<float> real(size / 2 + 1, 0);
        std::vector<float> imaginary(size / 2 + 1, 0);
        
        // Do the forward transform
        forward(input, real.data(), imaginary.data());
        
        interleave(real.begin(), real.end(), imaginary.begin(), output);
    }
    
    template <class ComplexIterator>
    void FastFourierTransformBase::forward(const double* input, ComplexIterator output)
    {
        static_assert(std::is_same<typename ComplexIterator::value_type, std::complex<double>>::value, "should be an iterator to std::complex<double>");
        
        // The deinterleaved output will be stored in here
        std::vector<double> real(size / 2 + 1, 0);
        std::vector<double> imaginary(size / 2 + 1, 0);
        
        // Do the forward transform
        forward(input, real.data(), imaginary.data());
        
        interleave(real.begin(), real.end(), imaginary.begin(), output);
    }
    
    template <class ComplexIterator>
    std::vector<typename ComplexIterator::value_type::value_type> FastFourierTransformBase::inverse(ComplexIterator input)
    {
        std::vector<typename ComplexIterator::value_type::value_type> output(size);
        inverse(input, output.data());
        return output;
    }
    
    template <class ComplexIterator>
    void FastFourierTransformBase::inverse(ComplexIterator input, float* output)
    {
        static_assert(std::is_same<typename ComplexIterator::value_type, std::complex<float>>::value, "should be an iterator to std::complex<float>");
        
        // The deinterleaved input is stored in here
        std::vector<float> real(size / 2 + 1);
        std::vector<float> imaginary(size / 2 + 1);
        
        // Deinterleave
        deinterleave(input, input + size, real.begin(), imaginary.begin());
        
        // Do the inverse transform
        inverse(real.data(), imaginary.data(), output);
    }
    
    template <class ComplexIterator>
    void FastFourierTransformBase::inverse(ComplexIterator input, double* output)
    {
        static_assert(std::is_same<typename ComplexIterator::value_type, std::complex<double>>::value, "should be an iterator to std::complex<double>");
        
        // The deinterleaved input is stored in here
        std::vector<double> real(size / 2 + 1);
        std::vector<double> imaginary(size / 2 + 1);
        
        // Deinterleave
        deinterleave(input, input + size, real.begin(), imaginary.begin());
        
        // Do the inverse transform
        inverse(real.data(), imaginary.data(), output);
    }
    
    template <class ComplexIterator>
    std::vector<std::complex<typename ComplexIterator::value_type::value_type>> FastFourierTransformBase::forwardComplex(ComplexIterator input)
    {
        std::vector<std::complex<typename ComplexIterator::value_type::value_type>> output(size);
        forwardComplex(input, output.begin());
        return output;
    }
    
    template <class ComplexInputIterator, class ComplexOutputIterator>
    void FastFourierTransformBase::forwardComplex(ComplexInputIterator input, ComplexOutputIterator output)
    {
        // The deinterleaved input is stored in here
        std::vector<typename ComplexInputIterator::value_type::value_type> inputReal(size);
        std::vector<typename ComplexInputIterator::value_type::value_type> inputImaginary(size);
        
        // Deinterleave
        deinterleave(input, input + size, inputReal.begin(), inputImaginary.begin());
        
        std::vector<typename ComplexOutputIterator::value_type::value_type> outputReal(size);
        std::vector<typename ComplexOutputIterator::value_type::value_type> outputImaginary(size);
        
        // Do the inverse transform
        forwardComplex(inputReal.data(), inputImaginary.data(), outputReal.data(), outputImaginary.data());
        
        interleave(outputReal.begin(), outputReal.end(), outputImaginary.begin(), output);
    }
    
    template <class ComplexIterator>
    std::vector<std::complex<typename ComplexIterator::value_type::value_type>> FastFourierTransformBase::inverseComplex(ComplexIterator input)
    {
        std::vector<std::complex<typename ComplexIterator::value_type::value_type>> output(size);
        
        inverseComplex(input, output.begin());
        
        return output;
    }
    
    template <class ComplexInputIterator, class ComplexOutputIterator>
    void FastFourierTransformBase::inverseComplex(ComplexInputIterator input, ComplexOutputIterator output)
    {
        // The deinterleaved input is stored in here
        std::vector<typename ComplexInputIterator::value_type::value_type> inputReal(size);
        std::vector<typename ComplexInputIterator::value_type::value_type> inputImaginary(size);
        
        // Deinterleave
        deinterleave(input, input + size, inputReal.begin(), inputImaginary.begin());
        
        std::vector<typename ComplexOutputIterator::value_type::value_type> outputReal(size);
        std::vector<typename ComplexOutputIterator::value_type::value_type> outputImaginary(size);
        
        // Do the inverse transform
        inverseComplex(inputReal.data(), inputImaginary.data(), outputReal.data(), outputImaginary.data());
        
        interleave(outputReal.begin(), outputReal.end(), outputImaginary.begin(), output);
    }
}

#endif
