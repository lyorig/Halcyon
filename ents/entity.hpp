#pragma once

#include <ents/components.hpp>
#include <halcyon/ecs.hpp>
#include <lyo/bitset.hpp>

// entity.hpp:
// The base entity class.

namespace hq
{
    using ecs = hal::static_ecs<1024, position, velocity>;

    class entity
    {
    public:
        // Tag type for construction.
        template <typename... Comps>
        struct init
        {
        };

        template <typename... Comps>
        constexpr entity(init<Comps...>)
        {
        }

        template <typename C>
        constexpr void add_component(ecs& sys)
        {
            m_comps[ecs::id<C>()] = sys.component_add<C>();
        }

        template <typename C>
        constexpr void remove_component(ecs& sys)
        {
            constexpr auto id = ecs::id<C>();

            sys.component_remove<C>(m_comps[ecs::id<C>()]);
            m_comps[id] = no_component;
        }

        template <typename C>
        constexpr bool has_component(ecs& sys)
        {
            return m_comps[ecs::id<C>()] != no_component;
        }

    private:
        enum
        {
            no_component = std::numeric_limits<hal::component::index>::max()
        };

        std::array<hal::component::index, ecs::num_components()> m_comps;
    };
}