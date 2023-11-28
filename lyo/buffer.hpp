#pragma once

#include <algorithm>
#include <cstdio>

#include "capsule.hpp"

namespace lyo {
TAG_TYPE(zero_out);

template <typename T>
class buffer {
public:
    constexpr buffer() noexcept
        : m_data { nullptr }
        , m_size { 0 }
    {
    }

    constexpr buffer(usize sz) noexcept
        : m_data { new T[sz] }
        , m_size { sz }
    {
    }

    constexpr buffer(usize sz, zero_out_tag) noexcept
        : m_data { new T[sz] {} }
        , m_size { sz }
    {
    }

    // Getters.

    constexpr T& operator[](usize pos) noexcept
    {
        assert(pos < m_size);

        return m_data[pos];
    }

    constexpr const T& operator[](usize pos) const noexcept
    {
        assert(pos < m_size);

        return m_data[pos];
    }

    // Modifiers and mutators.

    constexpr void resize(usize sz) noexcept
    {
        m_data = new T[sz];
        m_size = sz;
    }

    constexpr void resize(usize sz, zero_out_tag) noexcept
    {
        m_data = new T[sz] {};
        m_size = sz;
    }

    constexpr void resize_move(usize sz) noexcept
    {
        const usize new_size { std::min(sz, m_size) };

        capsule<T[]> temp { std::move(m_data) };

        m_data = new T[sz];

        std::copy(temp.get(), temp.get() + new_size, m_data.get());

        m_size = sz;
    }

    constexpr void resize_move(usize sz, zero_out_tag) noexcept
    {
        const usize new_size { std::min(sz, m_size) };

        capsule<T[]> temp { std::move(m_data) };

        m_data = new T[sz] {};

        std::copy(temp.get(), temp.get() + new_size, m_data.get());

        m_size = sz;
    }

    template <typename... Args>
    constexpr void emplace(usize pos, Args... args) noexcept
    {
        assert(pos < m_size);

        new (m_data.get() + pos) T { args... };
    }

    constexpr usize size() const noexcept
    {
        return m_size;
    }

    // Range-based for-loop helpers.

    constexpr T* begin() noexcept
    {
        return m_data.get();
    }

    constexpr T* end() noexcept
    {
        return m_data.get() + m_size;
    }

    constexpr const T* begin() const noexcept
    {
        return m_data.get();
    }

    constexpr const T* end() const noexcept
    {
        return m_data.get() + m_size;
    }

    constexpr T* data() noexcept
    {
        return m_data.get();
    }

    constexpr const T* data() const noexcept
    {
        return m_data.get();
    }

private:
    capsule<T[]> m_data;
    usize m_size;
};
} // namespace lyo