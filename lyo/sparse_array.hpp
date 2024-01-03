#pragma once

#include <algorithm>
#include <array>

namespace lyo
{
    template <typename T, std::size_t Size, T Empty_Value>
    class sparse_array : std::array<T, Size>
    {
        using Base = std::array<T, Size>;

        template <typename Iter_type>
        class sparse_iterator
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = Iter_type;
            using pointer           = value_type*;
            using reference         = value_type&;

            sparse_iterator(pointer ptr, pointer end)
                : m_ptr { ptr }
                , m_end { end }
            {
                if (*ptr == Empty_Value)
                    ++(*this);
            }

            reference operator*() const { return *m_ptr; }
            pointer   operator->() { return m_ptr; }

            sparse_iterator& operator++()
            {
                do
                {
                    ++m_ptr;
                } while (*m_ptr == Empty_Value && m_ptr != m_end);

                return *this;
            }

            sparse_iterator operator++(int)
            {
                sparse_iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const sparse_iterator& a, const sparse_iterator& b) { return a.m_ptr == b.m_ptr; };
            friend bool operator!=(const sparse_iterator& a, const sparse_iterator& b) { return a.m_ptr != b.m_ptr; };

        private:
            pointer m_ptr;
            pointer m_end;
        };

    public:
        using iterator       = sparse_iterator<T>;
        using const_iterator = sparse_iterator<const T>;

        sparse_array()
        {
            this->clear();
        }

        void add(const T& value)
        {
            T* ptr { this->find(Empty_Value) };

            if (ptr == Base::end())
                return;

            *ptr = value;
        }

        void remove(const T& value)
        {
            T* ptr { this->find(value) };

            if (ptr == Base::end())
                return;

            *ptr = Empty_Value;
        }

        void clear()
        {
            std::fill(Base::begin(), Base::end(), Empty_Value);
        }

        bool has(const T& value) const
        {
            return this->find(value) != Base::end();
        }

        iterator begin()
        {
            return { Base::begin(), Base::end() };
        }

        const_iterator begin() const
        {
            return { Base::begin(), Base::end() };
        }

        iterator end()
        {
            return { Base::end(), Base::end() };
        }

        const_iterator end() const
        {
            return { Base::end(), Base::end() };
        }

    private:
        T* find(const T& value)
        {
            return std::find(Base::begin(), Base::end(), value);
        }

        const T* find(const T& value) const
        {
            return std::find(Base::begin(), Base::end(), value);
        }
    };
}