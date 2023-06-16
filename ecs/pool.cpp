#include "pool.hpp"

#include "entity.hpp"

using namespace ecs;

pool::pool(lyo::u16 elem_size) noexcept :
    m_buffer { elem_size * COMPONENT_SPACE },
    m_elementSize { elem_size }
{
}

void* pool::operator[](size_type idx) noexcept
{
    return m_buffer + idx * m_elementSize;
}