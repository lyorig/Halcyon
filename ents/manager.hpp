#pragma once

#include <ents/entity.hpp>
#include <vector>

// manager.hpp:
// HalQ's entity manager.

namespace hq
{
    class entity_manager
    {
        std::vector<entity> ents;
    };
}