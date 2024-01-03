#pragma once

#include <HalQ/entity.hpp>
#include <lyo/packed_array.hpp>

// manager.hpp:
// HalQ's entity manager.

namespace hq
{
    class manager
    {
        using vec = lyo::packed_array<entity, 128>;

    public:
        template <typename... Comps>
        constexpr entity::ID add()
        {
            m_ents.push_back(entity::create<Comps...>(m_ecs, m_ents.size(), {}));
            return m_ents.back().id();
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
        ecs m_ecs;
        vec m_ents;
    };
}