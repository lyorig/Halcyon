#pragma once

#include <algorithm>
#include <array>
#include <cassert>

namespace lyo
{
    template <typename T, std::size_t Size>
    class packed_array
    {
    public:
        using iterator       = T*;
        using const_iterator = const T*;

        using reference       = T&;
        using const_reference = const T&;

        constexpr void push_back(const T& value)
        {
            assert(!this->full());

            m_array[m_size++] = value;
        }

        constexpr void push_back(T&& value)
        {
            assert(!this->full());

            m_array[m_size++] = std::move(value);
        }

        constexpr void erase(iterator pos)
        {
            assert(pos >= this->begin() && pos < this->end());

            std::rotate(pos, pos + 1, this->end());

            --m_size;
        }

        constexpr void clear()
        {
            m_size = 0;
        }

        constexpr bool has(const T& value) const
        {
            return std::find(this->begin(), this->end(), value) != this->end();
        }

        constexpr bool full() const
        {
            return m_size == m_array.size();
        }

        constexpr std::size_t size() const
        {
            return m_size;
        }

        constexpr iterator begin()
        {
            return m_array.begin();
        }

        constexpr const_iterator begin() const
        {
            return m_array.begin();
        }

        constexpr iterator end()
        {
            return m_array.begin() + m_size;
        }

        constexpr const_iterator end() const
        {
            return m_array.begin() + m_size;
        }

        constexpr reference front()
        {
            return m_array[0];
        }

        constexpr const_reference front() const
        {
            return m_array[0];
        }

        constexpr reference back()
        {
            return m_array[m_size - 1];
        }

        constexpr const_reference back() const
        {
            return m_array[m_size - 1];
        }

        constexpr reference operator[](std::size_t idx)
        {
            return m_array[idx];
        }

        constexpr const_reference operator[](std::size_t idx) const
        {
            return m_array[idx];
        }

    private:
        std::array<T, Size> m_array;
        std::size_t         m_size { 0 };
    };
}