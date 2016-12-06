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

#ifndef GRIZZLY_CIRCULAR_BUFFER_HPP
#define GRIZZLY_CIRCULAR_BUFFER_HPP

#include <cstddef>
#include <dsperados/math/utility.hpp>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace dsp
{
    //! A buffer with a set capacity, overwriting the oldest samples if it runs out of space
    template <class T>
    class CircularBuffer
    {
    public:
        template <class BufferType, class PointerType, class ReferenceType>
        class Iterator
        {
            friend class CircularBuffer;
            
        public:
            using iterator_category = std::random_access_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = PointerType;
            using reference = ReferenceType;
            
        public:
            //! Dereference the iterator
            ReferenceType operator*() { return buffer[index]; }
            
            //! Dereference the iterator
            const ReferenceType operator*() const { return buffer[index]; }
            
            //! Access a member in the pointee
            PointerType operator->() { return &buffer[index]; }
            
            //! Access a member in the pointee
            const PointerType operator->() const { return &buffer[index]; }
            
            //! Increment the iterator
            Iterator& operator++() { ++index; return *this; }
            
            //! Increment the iterator
            Iterator& operator++(int) { ++index; return *this; }
            
            //! Decrement the iterator
            Iterator& operator--() { --index; return *this; }
            
            //! Decrement the iterator
            Iterator& operator--(int) { --index; return *this; }
            
            //! Add a distance to this iterator
            Iterator& operator+=(std::ptrdiff_t distance) { index += distance; return *this; }
            
            //! Subtract a distance from this iterator
            Iterator& operator-=(std::ptrdiff_t distance) { index -= distance; return *this; }
            
            //! Subtract another iterator from this one (computing the distance)
            std::ptrdiff_t operator-(const Iterator& rhs) { return index - rhs.index; }
            
            //! Compare two iterators for equality
            bool operator==(const Iterator& rhs) const { return &buffer == &rhs.buffer && index == rhs.index; }
            
            //! Compare two iterators for inequality
            bool operator!=(const Iterator& rhs) const { return !(*this == rhs); }
            
        protected:
            Iterator(typename std::add_lvalue_reference<BufferType>::type& buffer, std::ptrdiff_t index) :
                buffer(buffer),
                index(index)
            { }
            
        private:
            typename std::add_lvalue_reference<BufferType>::type buffer;
            std::ptrdiff_t index = 0;
        };
        
        using iterator = Iterator<CircularBuffer, T*, T&>;
        using const_iterator = Iterator<const CircularBuffer, const T*, const T&>;
        
    public:
        //! Construct the circular buffer with a given size
        CircularBuffer(std::size_t size) :
            data(size)
        {
            
        }
        
        //! Construct the buffer by feeding its samples directly
        CircularBuffer(std::initializer_list<T> elements) :
            data{elements}
        {
            
        }
        
        //! Construct the buffer from an iterator range
        template <typename Iterator>
            CircularBuffer(Iterator begin, Iterator end) :
            data(begin, end)
        {
            
        }
        
        //! Put a new value at the back of the buffer
        template <class... Args>
        void emplace_back(Args&&... args)
        {
            data[front] = T(std::forward<Args&&>(args)...);
            front = math::wrap<std::size_t>(front + 1, 0, data.size());
        }
        
        //! Access one of the elements in the buffer
        T& operator[](std::size_t index)
        {
            if (index >= data.size())
                throw std::out_of_range("circular buffer index (" + std::to_string(index) + ") >= size (" + std::to_string(data.size()) + ")");
            
            return data[math::wrap<std::size_t>(front + index, 0, data.size())];
        }
        
        //! Access one of the elements in the buffer
        const T& operator[](std::size_t index) const
        {
            if (index >= data.size())
                throw std::out_of_range("circular buffer index (" + std::to_string(index) + ") >= size (" + std::to_string(data.size()) + ")");
            
            return data[math::wrap<std::size_t>(front + index, 0, data.size())];
        }
        
        //! Resize the buffer
        void resize_back(std::size_t newSize)
        {
            std::vector<T> newData(begin(), end());
            newData.resize(newSize);
            data = newData;
        }
        
        //! Resize the buffer
        void resize_front(std::size_t newSize)
        {
            std::vector<T> newData(rbegin(), rend());
            newData.resize(newSize);
            std::reverse(newData.begin(), newData.end());
            data = newData;
        }
        
        //! Return the size of the buffer
        std::size_t size() const { return data.size(); }
        
        // Begin and end for ranged for-loops
        iterator begin() { return {*this, 0}; }
        const_iterator begin() const { return {*this, 0}; }
        const_iterator cbegin() const { return {*this, 0}; }
        
        iterator end() { return {*this, static_cast<std::ptrdiff_t>(data.size())}; }
        const_iterator end() const { return {*this, static_cast<std::ptrdiff_t>(data.size())}; }
        const_iterator cend() const { return {*this, static_cast<std::ptrdiff_t>(data.size())}; }
        
        std::reverse_iterator<iterator> rbegin() { return std::reverse_iterator<iterator>(end()); }
        std::reverse_iterator<const_iterator> rbegin() const { return std::reverse_iterator<const_iterator>(cend()); }
        std::reverse_iterator<const_iterator> crbegin() const { return std::reverse_iterator<const_iterator>(cend()); }
        
        std::reverse_iterator<iterator> rend() { return std::reverse_iterator<iterator>(begin()); }
        std::reverse_iterator<const_iterator> rend() const { return std::reverse_iterator<const_iterator>(cbegin()); }
        std::reverse_iterator<const_iterator> crend() const { return std::reverse_iterator<const_iterator>(cbegin()); }
        
    private:
        //! The actual buffer
        std::vector<T> data;
        
        //! The index pointing to the front of the buffer
        std::size_t front = 0;
    };
}

#endif /* DSP_CIRCULAR_BUFFER_HPP */
