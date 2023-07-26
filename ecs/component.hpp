#pragma once

#include <bitset>
#include <lyo/types.hpp>

namespace ecs
{
    namespace component
    {
        using index = lyo::u8;

        // The maximum amount of unique components, deciding bitset size.
        // Set this as low as possible, so as to save space.
        constexpr index max { 16 };

        using mask = std::bitset<max>;
    }  // namespace component
}  // namespace ecs