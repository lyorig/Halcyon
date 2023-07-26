#include "camera.hpp"

#include <halcyon/debug.hpp>

using namespace game;

namespace
{
    constexpr double default_camera_speed { 30.0 };
}

camera::camera() noexcept :
    scale { 1.0 },
    speed { default_camera_speed },
    m_target { nullptr }
{
}

camera::camera(lyo::f64 speed) noexcept :
    scale { 1.0 },
    speed { speed },
    m_target { nullptr }
{
}

void camera::set_target(const entity& ent) noexcept
{
    m_target = &ent;
}

void camera::update(lyo::f64 time) noexcept
{
    HAL_DEBUG_CHECK(m_target != nullptr, "Camera doesn't have a target while updating");

    // const lyo::f64 some_bezier_bs { time * time * (3.0 - 2.0 * time) };
}