#include "spritesheet.hpp"

#include "texture.hpp"

using namespace hal;

spritesheet::spritesheet(const texture& tx, const pixel_size& frame_size) noexcept :
    m_size { static_cast<frame_t>(tx.size().x / frame_size.x), static_cast<frame_t>(tx.size().y / frame_size.y) },
    m_frames { static_cast<lyo::usize>(m_size.x * m_size.y) }
{
    HAL_DEBUG_CHECK(tx.size().x % frame_size.x == 0 && tx.size().y % frame_size.y == 0, "Uneven spritesheet");

    HAL_DEBUG_PRINT(severity::info, "Created new spritesheet (texture size ", tx.size().x, 'x', tx.size().y, ", frame size ", frame_size.x, 'x', frame_size.y, ')');

    // HALFIX: Indexing error or some such

    for (frame_t y { 0 }; y < m_size.y; ++y)
        for (frame_t x { 0 }; x < m_size.x; ++x)
            m_frames.emplace(x + y * m_size.x, static_cast<pixel_type>(frame_size.x * x), static_cast<pixel_type>(frame_size.y * y), frame_size.x, frame_size.y);
}

const pixel_area& spritesheet::operator()(frame_t row, frame_t column) const noexcept
{
    const auto idx { row + column * m_size.x };

    HAL_DEBUG_CHECK(idx < m_size.x * m_size.y, "Out-of-bounds spritesheet access");

    return m_frames[idx];
}

frame_size spritesheet::size() const noexcept
{
    return m_size;
}

const lyo::buffer<pixel_area>& spritesheet::frames() const noexcept
{
    return m_frames;
}