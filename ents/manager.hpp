#pragma once

#include <ents/entity.hpp>
#include <vector>

// manager.hpp:
// HalQ's entity manager.

namespace hq
{
    class entity_manager
    {
    public:
    private:
        std::vector<entity> m_ents;
    };
}