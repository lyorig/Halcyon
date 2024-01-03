#pragma once

#include <halcyon/ecs.hpp>
#include <halcyon/types/render.hpp>

// components.hpp:
// All components used in the HalQ project.

namespace hq
{
    struct position : public hal::coord
    {
    };

    struct velocity : public hal::coord
    {
    };

    using ecs = hal::static_ecs<
        hal::comp::info<position, 20>,
        hal::comp::info<velocity, 20>>;
}