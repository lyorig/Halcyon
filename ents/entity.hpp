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
        
        lyo::bitset<ecs::num_components()> m_comps;
    };
}