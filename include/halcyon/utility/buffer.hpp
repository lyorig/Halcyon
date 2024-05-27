#pragma once

#include <memory>
#include <span>

#include <halcyon/utility/concepts.hpp>

namespace hal
{
    template <typename T>
    class buffer
    {
    public:
        buffer() = default;

        // Copy data from a span.
        buffer(std::span<const T> span)
            : m_arr { std::make_unique<T[]>(span.size()) }
            , m_size { span.size() }
        {
            std::copy(span.begin(), span.end(), begin());
        }

        std::size_t size() const
        {
            return m_size;
        }

        T& operator[](std::size_t idx)
        {
            return m_arr[idx];
        }

        const T& operator[](std::size_t idx) const
        {
            return m_arr[idx];
        }

        T* begin()
        {
            return m_arr.get();
        }

        const T* begin() const
        {
            return m_arr.get();
        }

        T* end()
        {
            return begin() + size();
        }

        const T* end() const
        {
            return begin() + size();
        }

    private:
        std::unique_ptr<T[]> m_arr;
        std::size_t          m_size;
    };
}