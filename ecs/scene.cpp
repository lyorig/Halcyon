#include "scene.hpp"

using namespace ecs;

entity::ID scene::new_entity() noexcept
{
    // We can recycle.
    if (!m_free.empty())
    {
        const entity::index new_idx { m_free.back() };

        m_free.pop_back();

        entity::ID& eid { m_ents[new_idx].id };

        eid.idx = new_idx;

        return eid;
    }

    // Recycling is not an option.
    return m_ents.emplace_back(m_ents.size(), 0).id;
}