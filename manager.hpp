#pragma once

#include <ecs/entity.hpp>
#include <lyo/static_vector.hpp>
#include <numeric>
#include <types.hpp>

// manager.hpp:
// HalQ's entity manager.

namespace hq
{
    template <std::size_t Max_Ents>
    class manager
    {
        template <typename T>
        using vec = lyo::static_vector<T, Max_Ents>;

    public:
        using entity = ECS::static_entity<holder>;

        enum : std::size_t
        {
            max_ents = Max_Ents
        };

        manager()
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
            (ent.add<Comps>(m_holder), ...);

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
            this->free_entity(eid, holder::type_list {});

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
            const entity::container& c { this->operator[](eid).comps() };

            (m_holder.remove<Ts>(c[holder::id<Ts>()]), ...);
        }

        holder m_holder;

        vec<entity>     m_ents;
        vec<entity::ID> m_free;
    };
}