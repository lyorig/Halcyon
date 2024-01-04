#pragma once

#include <ecs/entity.hpp>
#include <lyo/static_vector.hpp>
#include <numeric>
#include <types.hpp>

// manager.hpp:
// HalQ's entity manager.

namespace ECS
{
    template <typename Comp_Mgr, std::size_t Max_Ents>
    class static_entity_manager
    {
        template <typename T>
        using vec = lyo::static_vector<T, Max_Ents>;

    public:
        using entity = static_entity<Comp_Mgr>;

        enum : std::size_t
        {
            max_ents = Max_Ents
        };

        static_entity_manager()
            : m_free { max_ents }
        {
            std::iota(m_free.begin(), m_free.end(), 0);
        }

        template <typename... Comps>
        constexpr entity::ID spawn()
        {
            m_ents.emplace_back(m_free.back());
            m_free.pop_back();

            entity& ent { m_ents.back() };
            (ent.template add<Comps>(m_holder), ...);

            return ent.id();
        }

        template <typename... Comps>
        constexpr bool has(entity::ID eid)
        {
            return m_ents[eid].template has<Comps...>(m_holder);
        }

        constexpr void kill(entity::ID eid)
        {
            auto iter = std::find(m_ents.begin(), m_ents.end(), eid);
            assert(iter != m_ents.end());

            // I don't know why, but if I try setting the type list as
            // a default argument, compilation fails. I love templates!
            this->free_entity(eid, typename Comp_Mgr::type_list {});

            m_free.push_back(eid);
            m_ents.erase(iter);
        }

        constexpr entity& operator[](entity::ID eid)
        {
            auto iter = std::find(m_ents.begin(), m_ents.end(), eid);
            assert(iter != m_ents.end());

            return *iter;
        }

        constexpr const entity& operator[](entity::ID eid) const
        {
            const auto iter = std::find(m_ents.begin(), m_ents.end(), eid);
            assert(iter != m_ents.end());

            return *iter;
        }

    private:
        template <typename... Ts>
        constexpr void free_entity(entity::ID eid, lyo::type_list<Ts...>)
        {
            const typename entity::container& c { this->operator[](eid).comps() };

            (m_holder.template remove<Ts>(c[Comp_Mgr::template id<Ts>()]), ...);
        }

        Comp_Mgr m_holder;

        vec<entity>              m_ents;
        vec<typename entity::ID> m_free;
    };
}