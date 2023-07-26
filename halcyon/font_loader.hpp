#pragma once

#include "components/font.hpp"

namespace hal
{
    class window;

    // For loading fonts, duh. Make sure it outlives its loaded
    // fonts, as font destructors require the TTF library to be initialized.
    class font_loader
    {
      public:

        font_loader(window& wnd) noexcept;
        ~font_loader();

        font load(const char* path, lyo::u8 size) & noexcept;
    };
}  // namespace hal