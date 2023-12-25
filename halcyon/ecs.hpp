#pragma once

#include <algorithm>
#include <array>
#include <halcyon/debug.hpp>
#include <lyo/concepts.hpp>
#include <lyo/types.hpp>
#include <span>

// ecs.hpp:
// Halcyon's Entity-Component System implementation.

namespace hal
{
    namespace component
    {
        using id = lyo::u8;
        using index = lyo::u16;
    }

    namespace entity
    {
        using id = lyo::u16;
    }

    // This class attempts to be as constexpr-friendly as possible. Templates ahoy!
    // Arena_Size_Bytes: the size of the component arena in bytes.
    // Max_Registered_Components: the maximum amount of components one can register.
    // Both affect the resulting size of the class. Set according to your own needs.
    template <std::size_t Arena_Size_Bytes, typename... Cs>
        requires(Arena_Size_Bytes > 0 && sizeof...(Cs) > 0)
    class static_ecs
    {
    public:
        // The smaller the size type, the smaller the total size.
        using size_type = lyo::u16;

        template <std::convertible_to<component::index>... Sizes>
            requires(sizeof...(Sizes) == sizeof...(Cs))
        static_ecs(Sizes... sizes)
        {
            (this->add<Cs>(sizes), ...);

            HAL_DEBUG_PRINT(hal::debug::init, "ECS loaded with ", sizeof...(Cs), ' ', sizeof...(Cs) == 1 ? "component" : "components", ", totalling ", this->memory_used(), "B out of ", this->memory_total(), 'B');
        }

        template <lyo::one_of<Cs...> C>
        constexpr void remove()
        {
            constexpr auto id = static_ecs::id<C>();

            const auto amnt_bytes = this->space_bytes<C>();
            const auto begin = m_begins.begin() + id + 1;

            // Adjust sizes.
            std::for_each(begin, m_begins.end(), [](size_type& sz)
                { sz -= amnt_bytes; });
        }

        template <lyo::one_of<Cs...> C>
        constexpr std::span<C> get()
        {
            constexpr auto id = static_ecs::id<C>();

            C* begin { reinterpret_cast<C*>(m_arena.begin() + m_begins[id]) };

            return { begin, reinterpret_cast<C*>(&m_begins[id + 1]) };
        }

        template <lyo::one_of<Cs...> C>
        constexpr std::span<const C> get() const
        {
            constexpr auto id = static_ecs::id<C>();

            const C* begin { reinterpret_cast<const C*>(m_arena.begin() + m_begins[id]) };

            return { begin, reinterpret_cast<const C*>(&m_begins[id + 1]) };
        }

        template <lyo::one_of<Cs...> C>
        constexpr component::index space() const
        {
            return this->space_bytes<C>() / sizeof(C);
        }

        constexpr static std::size_t memory_total()
        {
            return Arena_Size_Bytes;
        }

        constexpr std::size_t memory_used() const
        {
            return *std::find_if(m_begins.crbegin(), m_begins.crend(), [](size_type val)
                { return val != 0; });
        }

        constexpr std::size_t memory_available()
        {
            return static_ecs::memory_total() - this->memory_used();
        }

        constexpr component::id registered() const
        {
            return m_currentID;
        }

        template <lyo::one_of<Cs...> C>
        consteval static component::id id()
        {
            return component::id(lyo::index_v<C, Cs...>);
        }

    private:
        template <lyo::one_of<Cs...> C>
        constexpr void add(component::index how_many)
        {
            constexpr auto id = static_ecs::id<C>();

            auto next = m_begins.begin() + id + 1;
            HAL_DEBUG_ASSERT(*next == 0, "Component already added");

            *next = *(next - 1) + how_many * sizeof(C);

            HAL_DEBUG_ASSERT(*next <= Arena_Size_Bytes, "Arena size exceeded");
        }

        template <lyo::one_of<Cs...> C>
        constexpr size_type space_bytes() const
        {
            constexpr auto id = static_ecs::id<C>();

            return m_begins[id + 1] - m_begins[id];
        }

        std::array<std::byte, Arena_Size_Bytes>  m_arena;
        std::array<size_type, sizeof...(Cs) + 1> m_begins {};

        component::id m_currentID { 0 };
    };
}