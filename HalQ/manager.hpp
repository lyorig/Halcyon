#pragma once

#include <HalQ/components.hpp>
#include <ecs/entity.hpp>
#include <lyo/static_vector.hpp>

// manager.hpp:
// HalQ's entity manager.

namespace hq
{
    template <std::size_t Max_Ents>
    class manager
    {
        using entity = ecs::static_entity<holder>;
        using vec    = lyo::static_vector<entity, Max_Ents>;

    public:
        template <typename... Comps>
        constexpr entity::ID spawn()
        {
            entity e { m_ents.size() };
            (e.add<Comps>(m_holder), ...);

            m_ents.push_back(std::move(e));

            return e.id();
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
        holder m_holder;

        vec m_ents;
    };
}