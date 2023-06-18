#pragma once

#include <SDL2/SDL_ttf.h>

#include <lyo/pass_key.hpp>
#include <lyo/types.hpp>

#include "sdl_object.hpp"

namespace hal
{
    class font_loader;

    class font : public sdl_object<TTF_Font, ::TTF_CloseFont>
    {
      public:

        font(const char* path, lyo::u8 size, lyo::pass_key<font_loader>) noexcept;
    };
}  // namespace hal