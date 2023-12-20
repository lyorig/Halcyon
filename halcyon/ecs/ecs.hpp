#pragma once

#include <algorithm>
#include <array>
#include <halcyon/debug.hpp>
#include <lyo/types.hpp>

// ecs.hpp:
// Halcyon's Entity-Component System implementation.

namespace hal
{
    namespace component
    {
        using id = lyo::u8;
        using index = lyo::u16;
    }

    // Arena_Size_Bytes: the size of the component arena in bytes.
    // Max_Registered_Components: the maximum amount of components one can register.
    // Both affect the resulting size of the class. Use according to your own needs.
    template <component::id Max_Registered_Components, std::size_t Arena_Size_Bytes>
        requires(Arena_Size_Bytes > 0 && Max_Registered_Components > 0)
    class ecs
    {
    public:
        template <typename Component>
        constexpr void add(component::index how_many)
        {
            const component::id id { this->id<Component>() };

            auto next = m_begins.begin() + id + 1;
            HAL_DEBUG_ASSERT(*next == 0, "Component already added");

            *next = *(next - 1) + how_many * sizeof(Component);

            HAL_DEBUG_ASSERT(*next <= Arena_Size_Bytes, "Arena size exceeded");

            HAL_DEBUG_PRINT(hal::debug::info, "Registered component: id ", lyo::u32(id), ", starts at ", *(next - 1));
        }

        template <typename Component>
        constexpr Component& get(component::index idx)
        {
            const auto id = this->id<Component>();
            return reinterpret_cast<Component*>(m_components.begin() + m_begins[id])[idx];
        }

        template <typename Component>
        constexpr component::index space()
        {
            const auto id = this->id<Component>();

            return (m_begins[id + 1] - m_begins[id]) / sizeof(Component);
        }

        static constexpr std::size_t memory_available()
        {
            return Arena_Size_Bytes;
        }

        constexpr std::size_t memory_used() const
        {
            return *std::find_if(m_begins.crbegin(), m_begins.crend(), [](std::size_t val)
                { return val != 0; });
        }

        constexpr component::id registered() const
        {
            return m_currentID;
        }

    private:
        template <typename Component>
        constexpr component::id id()
        {
            static component::id id { m_currentID++ };

            HAL_DEBUG_ASSERT(id < Max_Registered_Components, "Max registered component amount exceeded");

            return id;
        }

        // In the name of performance, this ECS implementation attempts
        // to keep all data on the stack.
        std::array<std::byte, Arena_Size_Bytes>             m_components;
        std::array<lyo::u32, Max_Registered_Components + 1> m_begins {};

        component::id m_currentID { 0 };
    };
}