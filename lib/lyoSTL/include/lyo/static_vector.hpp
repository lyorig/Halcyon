#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <lyo/tags.hpp>

namespace lyo
{
    template <typename T, std::size_t Max_Size, typename Size_Type = std::size_t>
        requires(Max_Size > 0 && Max_Size <= std::numeric_limits<Size_Type>::max())
    class static_vector
    {
    public:
        using value_type = T;
        using size_type  = Size_Type;

        using pointer       = T*;
        using const_pointer = const T*;

        using reference       = T&;
        using const_reference = const T&;

        using iterator       = T*;
        using const_iterator = const T*;

        constexpr static_vector() noexcept = default;

        constexpr static_vector(size_type sz) noexcept
        {
            this->resize(sz);
        }

        constexpr static_vector(size_type sz, LYO_TAG_NAME(no_init)) noexcept
        {
            this->resize(sz, tag::no_init);
        }

        constexpr static_vector(std::initializer_list<T> init) noexcept
            : m_size { init.size() }
        {
            assert(init.size() <= Max_Size);
            std::copy(init.begin(), init.end(), this->begin());
        }

        constexpr ~static_vector() noexcept
        {
            this->destroy_range(this->begin(), this->end());
        }

        constexpr void push_back(const T& value) noexcept
        {
            this->emplace_back(value);
        }

        constexpr void push_back(T&& value) noexcept
        {
            this->emplace_back(std::forward<T>(value));
        }

        template <typename... Args>
        constexpr void emplace(iterator pos, Args&&... args) noexcept
        {
            ++m_size;
            assert(this->size() <= capacity());
            assert(pos < this->end());

            std::shift_right(pos, this->end(), 1);
            new (pos) T { std::forward<Args>(args)... };
        }

        template <typename... Args>
        constexpr void emplace_back(Args&&... args) noexcept
        {
            this->emplace(this->begin() + m_size, std::forward<Args>(args)...);
        }

        constexpr void pop_back() noexcept
        {
            assert(m_size > 0);
            this->erase(this->end() - 1);
        }

        // Resize with a specified object.
        constexpr void resize(size_type sz, const T& obj = T {}) noexcept
        {
            if (sz == this->size()) // Nothing to do.
                return;

            if (sz == 0) // Full clear.
                return this->clear();

            if (sz < this->size()) // Need to remove elements.
                return this->erase(this->begin() + sz, this->end());

            // (sz > size()) Need to add elements.
            const size_type old_size { this->size() };
            m_size = sz;
            std::fill(this->begin() + old_size, this->end(), obj);
        }

        // Resize without initializing new objects.
        constexpr void resize(size_type sz, LYO_TAG_NAME(no_init)) noexcept
        {
            if (sz == this->size()) // Nothing to do.
                return;

            if (sz == 0) // Full clear.
                return this->clear();

            if (sz < this->size()) // Need to remove elements.
                return this->erase(this->begin() + sz, this->end());

            // (sz > size()) Need to add elements.
            m_size = sz;
        }

        constexpr void erase(iterator pos) noexcept
        {
            assert(pos >= this->begin() && pos < this->end());

            pos->~T();

            std::shift_left(pos, this->end(), 1);

            --m_size;
        }

        constexpr void erase(iterator begin, iterator end) noexcept
        {
            const size_type dist { size_type(std::distance(begin, end)) };

            if (dist == 0)
                return;

            assert(dist <= this->size());

            this->destroy_range(begin, end);

            std::shift_left(end, this->end(), dist);

            m_size -= dist;
        }

        constexpr void clear() noexcept
        {
            this->destroy_range(this->begin(), this->end());
            m_size = 0;
        }

        constexpr bool contains(const T& value) const noexcept
        {
            return std::find(this->begin(), this->end(), value) != this->end();
        }

        constexpr bool empty() const noexcept
        {
            return m_size == 0;
        }

        constexpr size_type size() const noexcept
        {
            return m_size;
        }

        constexpr static size_type capacity() noexcept
        {
            return Max_Size;
        }

        constexpr iterator begin() noexcept
        {
            return reinterpret_cast<iterator>(m_array.data());
        }

        constexpr const_iterator begin() const noexcept
        {
            return reinterpret_cast<const_iterator>(m_array.data());
        }

        constexpr iterator end() noexcept
        {
            return this->begin() + m_size;
        }

        constexpr const_iterator end() const noexcept
        {
            return this->begin() + m_size;
        }

        constexpr reference front() noexcept
        {
            assert(m_size > 0);
            return *this->begin();
        }

        constexpr const_reference front() const noexcept
        {
            assert(m_size > 0);
            return *this->begin();
        }

        constexpr reference back() noexcept
        {
            assert(m_size > 0);
            return *(this->end() - 1);
        }

        constexpr const_reference back() const noexcept
        {
            assert(m_size > 0);
            return *(this->end() - 1);
        }

        constexpr reference operator[](size_type idx) noexcept
        {
            assert(idx < m_size);
            return *(this->begin() + idx);
        }

        constexpr const_reference operator[](size_type idx) const noexcept
        {
            assert(idx < m_size);
            return *(this->begin() + idx);
        }

    private:
        void destroy_range(iterator begin, iterator end) noexcept
        {
            for (; begin != end; ++begin)
                begin->~T();
        }

        std::array<std::byte, Max_Size * sizeof(T)> m_array {};

        size_type m_size { 0 };
    };
}