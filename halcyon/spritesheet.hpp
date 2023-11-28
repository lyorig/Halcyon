#pragma once

#include <halcyon/types/render.hpp>
#include <lyo/buffer.hpp>

namespace hal
{
    using frame_t    = lyo::u8;
    using frame_size = point<frame_t>;
    using frame_pos  = frame_size;

    class texture;

    class spritesheet
    {
      public:

        spritesheet(const texture& tx, const pixel_size& frame_size) noexcept;

        // Drop-in replacement for a multidimensional subscript operator.
        const pixel_area& operator[](frame_pos pos) const noexcept;

        frame_size size() const noexcept;

        // Raw frame access - mostly for debugging.
        const lyo::buffer<pixel_area>& frames() const noexcept;

      private:

        frame_size m_size;

        lyo::buffer<pixel_area> m_frames;
    };
}  // namespace hal