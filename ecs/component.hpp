#pragma once

#include <limits>
#include <lyo/bitset.hpp>

/* components.hpp:
   [todo: finish description] A component doesn't have to
   be initialized, this is only for a unified style throughout the ECS. */

namespace ecs
{
    namespace component
    {
        using ID = lyo::u8;
    }

    constexpr component::ID max_components { 32 };

    static_assert(max_components <= std::numeric_limits<component::ID>::max(), "Maximum number of components out of data type range");

    namespace component
    {
        using mask = lyo::bitset<max_components>;
    }
}  // namespace ecs