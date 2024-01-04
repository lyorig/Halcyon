#pragma once

#include <array>
#include <bitset>
#include <cassert>
#include <ecs/info.hpp>

// ecs.hpp:
// An ECS' component manager implementation.
// Honestly, the static component manager is a goddamn abomination.
// In spite of this, however, I'm somewhat proud of myself for
// having made it. So, if all you want is speed, here it is.

namespace ECS
{
    // A fully stack-based component manager.
    // For template parameters, use a pack of comp::info structs.
    template <typename... Is>
        requires(sizeof...(Is) > 0 && sizeof...(Is) <= std::numeric_limits<comp::ID>::max())
    class static_component_manager
    {
    public:
        using type_list = lyo::type_list<typename Is::type...>;

        constexpr static_component_manager() = default;

        constexpr ~static_component_manager()
        {
            (this->clear<typename Is::type>(), ...);
        }

        template <comp::in_infos<Is...> C, typename... Args>
        constexpr comp::index add(Args&&... args)
        {
            constexpr std::size_t begin { comp::index_begin_v<C, Is...> };
            constexpr std::size_t end { begin + amount<C>() };

            const std::size_t idx { this->find_in_bitset<false>(begin, end) };

            assert(idx != end);

            new (m_arena.data() + comp::byte_begin_v<C, Is...>) C { std::forward(args)... };
            m_used.set(idx);

            return idx - begin;
        }

        template <comp::in_infos<Is...> C>
        constexpr void remove(comp::index idx)
        {
            if (idx == comp::invalid_index)
                return;

            assert(idx < amount<C>());

            constexpr std::size_t bgn { comp::index_begin_v<C, Is...> };

            if (!std::is_trivially_destructible_v<C>)
                this->get<C>(idx).~C();

            m_used.reset(bgn + idx);
        }

        template <comp::in_infos<Is...> C>
        constexpr void clear()
        {
            constexpr std::size_t bgn { comp::index_begin_v<C, Is...> };
            constexpr std::size_t end { bgn + amount<C>() };

            if (!std::is_trivially_destructible_v<C>)
                for (comp::index i { 0 }; i < amount<C>(); ++i)

                    for (std::size_t i { bgn }; i != end; ++i)
                        m_used.reset(i);
        }

        template <comp::in_infos<Is...> C>
        constexpr C& get(comp::index idx)
        {
            return type_begin<C>()[idx];
        }

        template <comp::in_infos<Is...> C>
        constexpr const C& get(comp::index idx) const
        {
            return type_begin<C>()[idx];
        }

        template <comp::in_infos<Is...> C>
        constexpr static comp::index amount()
        {
            return comp::associated_type_t<C, Is...>::amount();
        }

        constexpr static comp::index components()
        {
            return sizeof...(Is);
        }

        template <comp::in_infos<Is...> C>
        constexpr static comp::ID id()
        {
            return lyo::index_v<C, typename Is::type...>;
        }

    private:
        template <bool B>
        constexpr std::size_t find_in_bitset(std::size_t begin, std::size_t end)
        {
            for (; m_used[begin] != B && begin != end; ++begin)
                ;
            return begin;
        }

        template <comp::in_infos<Is...> C>
        constexpr C* type_begin()
        {
            return reinterpret_cast<C*>(m_arena.data() + comp::byte_begin_v<C, Is...>);
        }

        template <comp::in_infos<Is...> C>
        constexpr const C* type_begin() const
        {
            return reinterpret_cast<const C*>(m_arena.data() + comp::byte_begin_v<C, Is...>);
        }

        std::array<std::byte, comp::total_size_bytes_v<Is...>> m_arena;

        std::bitset<comp::total_amount_v<Is...>> m_used;
    };
}