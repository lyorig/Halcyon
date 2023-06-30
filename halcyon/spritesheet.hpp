#pragma once

#include <halcyon/types/render.hpp>
#include <lyo/buffer.hpp>

namespace hal
{
    using frame_t = lyo::u8;

    class texture;

    class spritesheet
    {
      public:

        spritesheet(const texture& tx, const pixel_size& frame_size) noexcept;

        // Drop-in replacement for a multidimensional subscript operator.
        const pixel_area& operator()(frame_t row, frame_t column) const noexcept;

      private:

        lyo::buffer<pixel_area> m_frames;
    };
}  // namespace hal