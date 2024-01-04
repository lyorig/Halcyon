#pragma once

#include <algorithm>
#include <ecs/info.hpp>
#include <lyo/pass_key.hpp>

// entity.hpp:
// The base entity class.

namespace ECS
{
    template <typename Container>
    class entity_base
    {
        using CID = comp::index;

    public:
        using ID        = lyo::u16;
        using container = Container;

        enum : ID
        {
            invalid_id = invalid<ID>
        };

        constexpr entity_base()
            : entity_base { invalid_id }
        {
        }

        constexpr entity_base(ID eid)
            : m_id { eid }
        {
        }

        template <typename C, typename System>
        constexpr void add(System& sys)
        {
            m_comps[sys.template id<C>()] = sys.template add<C>();
        }

        template <typename C, typename System>
        constexpr void remove(System& sys)
        {
            sys.template remove<C>(m_comps[sys.template id<C>()]);

            m_comps[sys.template id<C>()] = comp::invalid_index;
        }

        template <typename C, typename System>
        constexpr C& get(System& sys)
        {
            HAL_DEBUG_ASSERT(this->has<C>(sys), "Getting non-existent component");

            return sys.template get<C>()[this->cid<C>()];
        }

        template <typename C, typename System>
        constexpr const C& get(const System& sys) const
        {
            HAL_DEBUG_ASSERT(this->has<C>(sys), "Getting non-existent component");

            return sys.template get<C>(m_comps[sys.template id<C>()]);
        }

        template <typename System, typename... Cs>
        constexpr bool has(const System& sys)
        {
            return ((m_comps[sys.template id<Cs>()] != comp::invalid_index) && ...);
        }

        constexpr ID id() const
        {
            return m_id;
        }

        constexpr bool valid() const
        {
            return m_id != invalid_id;
        }

        const Container& comps()
        {
            return m_comps;
        }

    protected:
        template <typename C, typename System>
        comp::ID cid(const System& sys) { return m_comps[sys.template cid<C>()]; }

        Container m_comps;

    private:
        ID m_id;
    };

    template <typename Component_System>
    class static_entity : public entity_base<std::array<comp::index, Component_System::components()>>
    {
        using Base = entity_base<std::array<comp::index, Component_System::components()>>;

    public:
        static_entity(Base::ID eid)
            : Base { eid } // I hate this. I utterly despise this.
        {
            std::fill(Base::m_comps.begin(), Base::m_comps.end(), comp::invalid_index);
        }
    };

    template <typename Container>
    constexpr bool operator==(const entity_base<Container>& lhs, typename entity_base<Container>::ID rhs)
    {
        if (!lhs.valid() || rhs == entity_base<Container>::invalid_id)
            return false;

        return lhs.id() == rhs;
    }

    template <typename Container>
    constexpr bool operator==(const entity_base<Container>& lhs, const entity_base<Container>& rhs)
    {
        return lhs == rhs.id();
    }
}