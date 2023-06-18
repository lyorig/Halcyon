#pragma once

#include <unordered_map>

#include "components/font.hpp"

namespace hal
{
    class window;

    class font_loader
    {
      public:

        font_loader(window& wnd) noexcept;
        ~font_loader();

        font load(const char* path, lyo::u8 size) const noexcept;
    };
}  // namespace hal