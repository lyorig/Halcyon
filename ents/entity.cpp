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