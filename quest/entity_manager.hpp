#pragma once

#include "entities.hpp"

#include <variant>
#include <vector>

// entity_manager.hpp:
// A manager of entities... yeah.
// This, for whichever reason, is insanely fast. Maybe due to template
// black magic, or something. Either way, it's pretty sick.

namespace quest
{
    template <typename T>
    concept manager_compatible = std::derived_from<T, entity> && lyo::non_cv<T>;

    template <manager_compatible... Types>
    class entity_manager
    {
    public:
        using holder = std::variant<Types...>;

        template <lyo::one_of<Types...> Ent_Type>
        class iterator
        {
        public:
            using iterator_category = std::contiguous_iterator_tag;

            using value_type      = std::remove_cv_t<Ent_Type>;
            using difference_type = std::ptrdiff_t;

            using pointer   = value_type*;
            using reference = value_type&;

            constexpr iterator() = default;

            constexpr iterator(holder* p)
                : m_ptr { p }
            {
            }

            constexpr iterator& operator++()
            {
                ++m_ptr;
                return *this;
            }

            constexpr iterator operator++(int)
            {
                iterator ret { *this };
                ++m_ptr;

                return ret;
            }

            constexpr iterator& operator--()
            {
                --m_ptr;
                return *this;
            }

            constexpr iterator operator--(int)
            {
                iterator ret { *this };
                --m_ptr;

                return ret;
            }

            // Two-way arithmetic operators.
            constexpr difference_type operator-(const iterator& rhs) const
            {
                return m_ptr - rhs.m_ptr;
            }

            constexpr friend iterator operator-(const iterator& lhs, difference_type dist)
            {
                return lhs.m_ptr - dist;
            }

            constexpr friend iterator operator-(difference_type dist, const iterator& rhs)
            {
                return rhs.m_ptr - dist;
            }

            constexpr friend iterator operator+(const iterator& lhs, difference_type dist)
            {
                return lhs.m_ptr + dist;
            }

            constexpr friend iterator operator+(difference_type dist, const iterator& rhs)
            {
                return rhs.m_ptr + dist;
            }

            // Other arithmetic operators.
            constexpr iterator& operator+=(difference_type dist)
            {
                m_ptr += dist;
                return *this;
            }

            constexpr iterator& operator-=(difference_type dist)
            {
                m_ptr -= dist;
                return *this;
            }

            // Object access.
            constexpr reference operator*()
            {
                return std::get<lyo::index_v<value_type, Types...>>(*m_ptr);
            }

            constexpr reference operator*() const
            {
                return std::get<lyo::index_v<value_type, Types...>>(*m_ptr);
            }

            constexpr pointer operator->()
            {
                return &std::get<lyo::index_v<value_type, Types...>>(*m_ptr);
            }

            constexpr pointer operator->() const
            {
                return &std::get<lyo::index_v<value_type, Types...>>(*m_ptr);
            }

            constexpr reference operator[](std::size_t i)
            {
                return std::get<lyo::index_v<value_type, Types...>>(m_ptr[i]);
            }

            constexpr reference operator[](std::size_t i) const
            {
                return std::get<lyo::index_v<value_type, Types...>>(m_ptr[i]);
            }

            constexpr auto operator<=>(const iterator& other) const = default;

        private:
            holder* m_ptr;
        };

        template <lyo::one_of<Types...> Ent_Type, typename... Args>
        constexpr void spawn(Args&&... args)
        {
            m_vectors[lyo::index_v<Ent_Type, Types...>]
                .emplace_back(std::in_place_type<Ent_Type>, m_id++, std::forward<Args>(args)...);
        }

        // Do something with every entity of every type.
        // If you want to constrain the types, you can use a requires expression to do
        // so, for example to only use the function on types that have a certain method.
        template <typename Visitor>
        constexpr void visit(Visitor&& v)
        {
            ([this, v]()
                {
                    // Suppress warnings about unused captures.
                    (void)this, (void)v;

                    if constexpr (std::is_invocable_v<decltype(v), Types>)
                    {
                        for (auto& ent : view<Types>())
                            v(ent);
                    } }(),
                ...);
        }

        // Get access to a specific entity type.
        template <lyo::one_of<Types...> Ent_Type>
        constexpr std::span<Ent_Type> view()
        {
            std::vector<holder>& hld { m_vectors[lyo::index_v<Ent_Type, Types...>] };
            return { iterator<Ent_Type>(&*hld.begin()), hld.size() };
        }

        // Directly access the vector holding a specific entity type.
        template <lyo::one_of<Types...> Ent_Type>
        constexpr std::vector<holder>& access()
        {
            return m_vectors[lyo::index_v<Ent_Type, Types...>];
        }

    private:
        std::array<std::vector<holder>, sizeof...(Types)> m_vectors;

        entity::ID m_id { 0 };
    };
}