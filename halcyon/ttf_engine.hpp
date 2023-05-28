#pragma once

#include <unordered_map>

#include "components/font.hpp"
#include "components/surface.hpp"

namespace halcyon
{
    class engine;

    class ttf_engine
    {
      public:

        ttf_engine(engine& engine) noexcept;
        ~ttf_engine();

        font    load_font(const char* path, lyo::u8 size) const noexcept;
        surface load_text(const font& font, const char* text) const noexcept;
        surface load_text(const font& font, const char* text, color_t color) const noexcept;
    };
}  // namespace halcyon