#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <lyo/tags.hpp>

namespace lyo
{
    template <typename T, std::size_t Max_Size>
        requires(Max_Size > 0)
    class static_vector
    {
    public:
        using iterator       = T*;
        using const_iterator = const T*;

        using reference       = T&;
        using const_reference = const T&;

        constexpr static_vector() = default;

        constexpr static_vector(std::size_t sz)
        {
            this->resize(sz);
        }

        constexpr static_vector(std::size_t sz, no_init_tag)
        {
            this->resize(sz, no_init);
        }

        constexpr static_vector(std::initializer_list<T> init)
            : m_size { init.size() }
        {
            std::copy(init.begin(), init.end(), this->begin());
        }

        constexpr ~static_vector()
        {
            this->destroy_range(this->begin(), this->end());
        }

        constexpr void push_back(const T& value)
        {
            this->emplace_back(value);
        }

        constexpr void push_back(T&& value)
        {
            this->emplace_back(std::forward(value));
        }

        template <typename... Args>
        constexpr void emplace(iterator pos, Args&&... args)
        {
            ++m_size;
            assert(this->size() <= capacity());
            assert(pos < this->end());

            std::shift_right(pos, this->end(), 1);
            new (pos) T { std::forward<Args>(args)... };
        }

        template <typename... Args>
        constexpr void emplace_back(Args&&... args)
        {
            this->emplace(this->begin() + m_size, std::forward<Args>(args)...);
        }

        constexpr void pop_back()
        {
            assert(m_size > 0);
            this->erase(this->end() - 1);
        }

        // Resize with a specified object.
        constexpr void resize(std::size_t sz, const T& obj = T {})
        {
            if (sz == this->size()) // Nothing to do.
                return;

            if (sz == 0) // Full clear.
                return this->clear();

            if (sz < this->size()) // Need to remove elements.
                return this->erase(this->begin() + sz, this->end());

            // (sz > size()) Need to add elements.
            const std::size_t old_size { this->size() };
            m_size = sz;
            std::fill(this->begin() + old_size, this->end(), obj);
        }

        // Resize without initializing new objects.
        constexpr void resize(std::size_t sz, no_init_tag)
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

        constexpr void erase(iterator pos)
        {
            assert(pos >= this->begin() && pos < this->end());

            pos->~T();

            std::shift_left(pos, this->end(), 1);

            --m_size;
        }

        constexpr void erase(iterator begin, iterator end)
        {
            const std::size_t dist { std::size_t(std::distance(begin, end)) };

            if (dist == 0)
                return;

            assert(dist <= this->size());

            this->destroy_range(begin, end);

            std::shift_left(end, this->end(), dist);

            m_size -= dist;
        }

        constexpr void clear()
        {
            this->destroy_range(this->begin(), this->end());
            m_size = 0;
        }

        constexpr bool has(const T& value) const
        {
            return std::find(this->begin(), this->end(), value) != this->end();
        }

        constexpr bool empty() const
        {
            return m_size == 0;
        }

        constexpr std::size_t size() const
        {
            return m_size;
        }

        constexpr static std::size_t capacity()
        {
            return Max_Size;
        }

        constexpr iterator begin()
        {
            return reinterpret_cast<iterator>(m_array.begin());
        }

        constexpr const_iterator begin() const
        {
            return reinterpret_cast<const_iterator>(m_array.begin());
        }

        constexpr iterator end()
        {
            return this->begin() + m_size;
        }

        constexpr const_iterator end() const
        {
            return this->begin() + m_size;
        }

        constexpr reference front()
        {
            assert(m_size > 0);
            return *this->begin();
        }

        constexpr const_reference front() const
        {
            assert(m_size > 0);
            return *this->begin();
        }

        constexpr reference back()
        {
            assert(m_size > 0);
            return *(this->end() - 1);
        }

        constexpr const_reference back() const
        {
            assert(m_size > 0);
            return *(this->end() - 1);
        }

        constexpr reference operator[](std::size_t idx)
        {
            assert(idx < m_size);
            return *(this->begin() + idx);
        }

        constexpr const_reference operator[](std::size_t idx) const
        {
            assert(idx < m_size);
            return *(this->begin() + idx);
        }

    private:
        void destroy_range(iterator begin, iterator end)
        {
            for (; begin != end; ++begin)
                begin->~T();
        }

        std::array<std::byte, Max_Size * sizeof(T)> m_array;

        std::size_t m_size { 0 };
    };
}