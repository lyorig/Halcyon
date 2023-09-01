#pragma once

#include <SDL2/SDL_ttf.h>

#include <lyo/pass_key.hpp>
#include <lyo/types.hpp>
#include <string>

#include "sdl_object.hpp"

namespace hal
{
    class font_loader;
    class surface;

    class font : public sdl_object<TTF_Font, &::TTF_CloseFont>
    {
      public:

        font(const char* path, lyo::u8 size, lyo::pass_key<font_loader>) noexcept;

        surface render(const std::string& text, color clr = color::white) const noexcept;

      private:

        constexpr static SDL_Color sdl_color_from(color_type clr) noexcept;
    };
}  // namespace hal