#pragma once

#include <limits>
#include <lyo/bitset.hpp>

/* A component doesn't have to be initialized, this is only
   for a unified style throughout the ECS. */

namespace ecs
{
    namespace component
    {
        using ID = lyo::u8;
    }

    constexpr component::ID MAX_COMPONENTS { 32 };

    static_assert(MAX_COMPONENTS <= std::numeric_limits<component::ID>::max(), "Maximum number of components out of data type range");

    namespace component
    {
        using mask = lyo::bitset<MAX_COMPONENTS>;
    }
}  // namespace ecs