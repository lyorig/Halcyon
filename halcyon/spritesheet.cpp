#include "spritesheet.hpp"

#include "texture.hpp"

using namespace hal;

spritesheet::spritesheet(const texture& tx, const pixel_size& frame_size) noexcept
{
    HAL_DEBUG_VERIFY(frame_size.x <= tx.size().x && frame_size.y <= tx.size().y, "Texture size < frame size");

    const frame_t anims { static_cast<frame_t>(tx.size().y / frame_size.y) };
    const frame_t frames { static_cast<frame_t>(tx.size().x / frame_size.x) };

    m_rects.resize(anims);

    for (frame_t y { 0 }; y < anims; ++y)
    {
        m_rects[y].reserve(frames);

        for (frame_t x { 0 }; x < frames; ++x)
            m_rects[y].emplace_back(frame_size.x * x, frame_size.y * y, frame_size.x, frame_size.y);
    }

    HAL_DEBUG_PRINT(debug::info, "Created new spritesheet (texture size ", tx.size().x, 'x', tx.size().y, ", frame size ", frame_size.x, 'x', frame_size.y, ')');
}

const std::vector<pixel_area>& spritesheet::operator[](frame_t animation) const noexcept
{
    return m_rects[animation];
}