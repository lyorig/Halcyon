#include "spritesheet.hpp"

#include "texture.hpp"

using namespace hal;

spritesheet::spritesheet(const texture& tx, const pixel_size& frame_size) noexcept
{
    HAL_DEBUG_CHECK(tx.size().x % frame_size.x == 0 && tx.size().y % frame_size.y == 0, "Uneven spritesheet");

    HAL_DEBUG_PRINT(severity::info, "Created new spritesheet (texture size ", tx.size().x, 'x', tx.size().y, ", frame size ", frame_size.x, 'x', frame_size.y, ')');

    // Untested code; don't be surprised if your PC catches fire.
    frame_t rows { static_cast<frame_t>(tx.size().x / frame_size.x) };
    frame_t cols { static_cast<frame_t>(tx.size().y / frame_size.y) };

    m_frames.resize(rows * cols);

    for (frame_t y { 0 }; y < cols; ++y)
    {
        for (frame_t x { 0 }; x < rows; ++y)
        {
            m_frames[x + y * cols] = { pixel_t(frame_size.x * x), pixel_t(frame_size.y * y), frame_size.x, frame_size.y };
        }
    }
}

const pixel_area& spritesheet::operator()(frame_t row, frame_t column) const noexcept
{
    return m_frames[row * column];
}