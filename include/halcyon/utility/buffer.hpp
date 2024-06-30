#pragma once

#include <memory>
#include <span>

#include <halcyon/utility/concepts.hpp>

namespace hal
{
    // A class that holds a heap-allocated buffer whose size won't change after construction.
    // So basically a vector, but without the need for storing a separate capacity.
    template <typename T>
    class buffer
    {
    public:
        constexpr buffer()
            : m_size { 0 }
        {
        }

        // Copy data from a span.
        constexpr buffer(std::span<const T> span)
            : m_arr { std::make_unique<T[]>(span.size()) }
            , m_size { span.size() }
        {
            std::copy(span.begin(), span.end(), begin());
        }

        constexpr std::size_t size() const
        {
            return m_size;
        }

        constexpr T& operator[](std::size_t idx)
        {
            return m_arr[idx];
        }

        constexpr const T& operator[](std::size_t idx) const
        {
            return m_arr[idx];
        }

        constexpr T* begin()
        {
            return m_arr.get();
        }

        constexpr const T* begin() const
        {
            return m_arr.get();
        }

        constexpr T* end()
        {
            return begin() + size();
        }

        constexpr const T* end() const
        {
            return begin() + size();
        }

    private:
        std::unique_ptr<T[]> m_arr;
        std::size_t          m_size;
    };
}