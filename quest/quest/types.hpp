#pragma once

#include <halcyon/types/render.hpp>
#include <lyoECS/component_manager.hpp>
#include <lyoSTL/timer.hpp>

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

    using holder = lyo::ECS::static_component_manager<lyo::ECS::info<position, 20>,
        lyo::ECS::info<velocity, 20>,
        lyo::ECS::info<timer, 10>>;
}