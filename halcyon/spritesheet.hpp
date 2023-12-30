#pragma once

#include <halcyon/types/render.hpp>
#include <lyo/buffer.hpp>

namespace hal
{
    using frame_type = lyo::u8;
    using frame_size = point<frame_type>;
    using frame_pos = frame_size;

    class texture_base;

    class spritesheet
    {
    public:
        spritesheet(const texture_base& tx, const pixel_size& frame_size);

        const pixel_area& operator[](frame_pos pos) const;

        frame_size size() const;

        // Raw frame access - mostly for debugging.
        const lyo::buffer<pixel_area>& frames() const;

    private:
        frame_size m_size;

        lyo::buffer<pixel_area> m_frames;
    };
} // namespace hal