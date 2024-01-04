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

    using holder = ECS::static_component_manager<ECS::comp::info<position, 20>,
        ECS::comp::info<velocity, 20>,
        ECS::comp::info<timer, 10>>;
}