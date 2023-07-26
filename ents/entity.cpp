#include "entity.hpp"

#include <halcyon/debug.hpp>

using namespace game;

entity::entity(const hal::coordinate& pos) noexcept :
    m_pos { pos }

{
}

entity::~entity()
{
}

void d1::update(lyo::f64 time) noexcept
{
    HAL_DEBUG_PRINT(hal::severity::info, "Updating D1 entity");
}

void d2::update(lyo::f64 time) noexcept
{
    HAL_DEBUG_PRINT(hal::severity::info, "Updating D2 entity");
}

bool d1::can_remove() const noexcept
{
    return false;
}

bool d2::can_remove() const noexcept
{
    return m_timer() >= 5.0;
}

d1::~d1()
{
    HAL_DEBUG_PRINT(hal::severity::info, "Destroying D1 entity");
}

d2::~d2()
{
    HAL_DEBUG_PRINT(hal::severity::info, "Destroying D2 entity");
}