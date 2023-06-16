#include "camera.hpp"

#include <halcyon/debug.hpp>

using namespace hq;

void camera::update() noexcept
{
    // Move towards an entity, preferably in an ease-out fashion.
}

void camera::set_target(const ent_base& ent) noexcept
{
    m_target = &ent;
}