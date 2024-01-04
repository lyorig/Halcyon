#pragma once

#include <ecs/component_manager.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/timer.hpp>

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

    using timer = lyo::precise_timer;

    using holder = ecs::static_component_manager<ecs::comp::info<position, 20>,
        ecs::comp::info<velocity, 20>,
        ecs::comp::info<timer, 10>>;
}