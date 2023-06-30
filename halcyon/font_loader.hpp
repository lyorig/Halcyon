#pragma once

#include <lyo/types.hpp>

namespace hal
{
    class window;
    class font;

    class font_loader
    {
      public:

        font_loader(window& wnd) noexcept;
        ~font_loader();

        font load(const char* path, lyo::u8 size) const noexcept &;
    };
}  // namespace hal