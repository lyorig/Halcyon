#pragma once

#include <halcyon/types/render.hpp>
#include <lyoECS/component_manager.hpp>
#include <lyoSTL/timer.hpp>

// components.hpp:
// All components used in the HalQ project.

namespace quest
{
    struct position : public hal::coord_point
    {
    };

    struct velocity : public hal::coord_point
    {
    };

    using timer = lyo::precise_timer;

    using holder = lyo::ecs::static_component_manager<lyo::ecs::info<position, 20>,
        lyo::ecs::info<velocity, 20>,
        lyo::ecs::info<timer, 10>>;
}