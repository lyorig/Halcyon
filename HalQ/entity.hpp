#pragma once

#include <HalQ/components.hpp>
#include <lyo/pass_key.hpp>

// entity.hpp:
// The base entity class.

namespace hq
{
    class manager;

    class entity
    {
        using CID = hal::comp::index;

    public:
        using ID = lyo::u16;

        enum : ID
        {
            invalid_id = std::numeric_limits<ID>::max()
        };

        template <typename... Comps>
        constexpr static entity create(ecs& sys, ID eid, lyo::pass_key<manager>)
        {
            entity e { eid };
            (e.add<Comps>(sys), ...);
            return e;
        }

        constexpr entity()
            : entity { invalid_id }
        {
        }

        template <typename C>
        constexpr void add(ecs& sys)
        {
            m_comps[ecs::component_ID<C>()] = sys.component_add<C>();
        }

        template <typename C>
        constexpr void remove(ecs& sys)
        {
            constexpr auto id = ecs::component_ID<C>();

            sys.component_remove<C>(m_comps[ecs::component_ID<C>()]);

            m_comps[id] = hal::comp::invalid_index;
        }

        template <typename C>
        constexpr C& get(ecs& sys)
        {
            HAL_DEBUG_ASSERT(this->has<C>(sys), "Getting non-existent component");

            return sys.get<C>()[this->cid<C>()];
        }

        template <typename C>
        constexpr const C& get(ecs& sys) const
        {
            HAL_DEBUG_ASSERT(this->has<C>(sys), "Getting non-existent component");

            return sys.get<C>()[this->cid<C>()];
        }

        template <typename C>
        constexpr bool has(ecs& sys)
        {
            return m_comps[ecs::component_ID<C>()] != hal::comp::invalid_index;
        }

        constexpr ID id() const
        {
            return m_id;
        }

        constexpr bool valid() const
        {
            return m_id != invalid_id;
        }

    private:
        constexpr entity(ID eid)
            : m_id { eid }
        {
            std::fill(m_comps.begin(), m_comps.end(), hal::comp::invalid_index);
        }

        template <typename T>
        constexpr CID cid()
        {
            return m_comps[ecs::component_ID<T>()];
        }

        std::array<CID, ecs::component_total_amount()> m_comps;

        ID m_id;
    };

    constexpr bool operator==(const entity& lhs, entity::ID rhs)
    {
        if (!lhs.valid() || rhs == entity::invalid_id)
            return false;

        return lhs.id() == rhs;
    }

    constexpr bool operator==(const entity& lhs, const entity& rhs)
    {
        return lhs == rhs.id();
    }
}