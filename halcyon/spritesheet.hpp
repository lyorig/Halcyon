#pragma once

#include <halcyon/types/types.hpp>
#include <vector>

namespace hal
{
    using frame_t = lyo::u8;

    class texture;

    class spritesheet
    {
      public:

        spritesheet(const texture& tx, const pixel_size& frame_size) noexcept;

        const std::vector<pixel_area>& operator[](frame_t animation) const noexcept;

      private:

        std::vector<std::vector<pixel_area>> m_rects;
    };
}  // namespace hal