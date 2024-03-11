#include <quest/spritesheet.hpp>

using namespace quest;

spritesheet::spritesheet(const hal::texture& tex, hal::pixel_point frame_size)
    : m_frameSize { frame_size }
    , m_size(tex.size() / frame_size)
{
    HAL_ASSERT((tex.size() % frame_size == hal::pixel_point { 0, 0 }), "Leftover texture data in sheet of size ", frame_size);

    hal::pixel_point* curr { m_buffer.get() };

    for (hal::pixel_point i { 0, 0 }; i.y != frame_size.y; i.y += frame_size.y)
        for (; i.x != frame_size.x; i.x += frame_size.x)
            *curr++ = i;
}

void spritesheet::next()
{
    m_curr.x = (m_curr.x + 1) % m_size.x;
}

hal::pixel_rect spritesheet::get() const
{
    return { m_buffer[m_curr.y * m_size.x + m_curr.x], m_frameSize };
}

frame_point spritesheet::size() const
{
    return m_size;
}

hal::pixel_point spritesheet::frame_size() const
{
    return m_frameSize;
}