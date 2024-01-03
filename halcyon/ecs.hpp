#pragma once

#include <algorithm>
#include <array>
#include <halcyon/debug.hpp>
#include <halcyon/internal/component.hpp>
#include <lyo/types.hpp>
#include <span>

// ecs.hpp:
// Halcyon's ECS component manager implementation.
// Honestly, the static_ecs class is a goddamn abomination.
// In spite of this, however, I'm somewhat proud of myself for
// having made it. So, if all you want is speed, here it is.

namespace hal
{
    // A fully stack-based component manager.
    // For template parameters, use a pack of hal::comp::info structs.
    template <typename... Is>
        requires(sizeof...(Is) > 0 && sizeof...(Is) <= std::numeric_limits<comp::ID>::max() && (std::is_trivially_destructible_v<typename Is::type>, ...))
    class static_ecs
    {
    public:
        constexpr static_ecs()
        {
            HAL_DEBUG_PRINT(hal::debug::init, "ECS loaded with ",
                to_printable_int(component_total_amount()), " comp(s), totalling ",
                m_arena.size(), "B and ",
                m_used.size(), " elements");

            // Extra verbose debug message if need be.
            // (HAL_DEBUG_PRINT("ECS: Comp. #", lyo::index_v<Cs, Cs...>, ", byte start = ", comp::byte_begin_v<typename Cs::type, Cs...>, ", index start = ", comp::index_begin_v<typename Cs::type, Cs...>), ...);
        }

        template <comp::in_infos<Is...> C, typename... Args>
        constexpr comp::index component_add(Args&&... args)
        {
            constexpr std::size_t begin { comp::index_begin_v<C, Is...> };
            constexpr std::size_t end { begin + component_amount<C>() };

            const std::size_t idx { this->find_in_bitset<false>(begin, end) };

            HAL_DEBUG_ASSERT(idx != end, "Component space exhausted, consider allocating more");

            this->get<C>(idx) = C { std::forward(args)... };
            m_used.set(idx);

            return idx - begin;
        }

        template <comp::in_infos<Is...> C>
        constexpr void component_remove(comp::index idx)
        {
            if (idx == comp::invalid_index)
                return;

            constexpr std::size_t bgn { comp::index_begin_v<C, Is...> };
            constexpr std::size_t num { component_amount<C>() };

            HAL_DEBUG_ASSERT(idx < num, "Removing invalid component index from ECS");

            m_used.reset(bgn + idx);
        }

        template <comp::in_infos<Is...> C>
        constexpr void component_clear()
        {
            constexpr std::size_t bgn { comp::index_begin_v<C, Is...> };
            constexpr std::size_t end { bgn + component_amount<C>() };

            for (std::size_t i { bgn }; i != end; ++i)
                m_used.reset(i);
        }

        template <comp::in_infos<Is...> C>
        consteval static comp::index component_amount()
        {
            return comp::associated_type_t<C, Is...>::amount();
        }

        consteval static comp::index component_total_amount()
        {
            return sizeof...(Is);
        }

        template <comp::in_infos<Is...> C>
        consteval static comp::ID component_ID()
        {
            return lyo::index_v<C, typename Is::type...>;
        }

    private:
        template <comp::in_infos<Is...> C>
        constexpr C& get(comp::index idx)
        {
            constexpr std::size_t begin { comp::byte_begin_v<C, Is...> };

            return reinterpret_cast<C&>(m_arena[begin + idx]);
        }

        template <comp::in_infos<Is...> C>
        constexpr const C& get(comp::index idx) const
        {
            constexpr std::size_t begin { comp::byte_begin_v<C, Is...> };

            return reinterpret_cast<const C&>(m_arena[begin + idx]);
        }

        template <bool B>
        constexpr std::size_t find_in_bitset(std::size_t begin, std::size_t end)
        {
            for (; m_used[begin] != B && begin != end; ++begin)
                ;
            return begin;
        }

        std::array<std::byte, comp::total_size_bytes_v<Is...>> m_arena;

        std::bitset<comp::total_amount_v<Is...>> m_used;
    };
}