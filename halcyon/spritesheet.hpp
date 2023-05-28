#pragma once

#include <vector>

#include "types.hpp"

namespace halcyon
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
}  // namespace halcyon