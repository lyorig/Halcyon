#pragma once

#include <algorithm>
#include <array>
#include <cassert>

namespace hal
{

    template <typename T, std::size_t Size, T Empty_Value>
    class packed_array
    {
    public:
        packed_array()
        {
            this->clear();
        }

        // Append an element to the array.
        // Please note that adding to a full array is allowed,
        // but attempting to add a duplicate element results in an error.
        void add(const T& value)
        {
            assert(!this->has(value));

            T* ptr { this->find(Empty_Value) };

            if (ptr == m_array.end())
                return;

            *ptr = value;
        }

        // Remove an element from the array.
        // Attempting to remove a non-present element is allowed.
        void remove(const T& value)
        {
            T* ptr { this->find(value) };

            if (ptr == m_array.end())
                return;

            *ptr = Empty_Value;
            std::rotate(ptr, ptr + 1, m_array.end());
        }

        // Check if the array has an element.
        bool has(const T& value) const
        {
            return this->find(value) != m_array.end();
        }

        // Clear the array.
        // This fills the underlying array with the user-specified empty value.
        void clear()
        {
            std::fill(m_array.begin(), m_array.end(), Empty_Value);
        }

        T* begin()
        {
            return m_array.begin();
        }

        const T* begin() const
        {
            return m_array.begin();
        }

        T* end()
        {
            return this->find(Empty_Value);
        }

        const T* end() const
        {
            return this->find(Empty_Value);
        }

    private:
        T* find(const T& value)
        {
            return std::find(m_array.begin(), m_array.end(), value);
        }

        const T* find(const T& value) const
        {
            return std::find(m_array.begin(), m_array.end(), value);
        }

        std::array<T, Size> m_array;
    };
}