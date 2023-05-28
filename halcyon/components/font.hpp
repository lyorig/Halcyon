#pragma once

#include <SDL2/SDL_ttf.h>

#include "sdl_object.hpp"

namespace halcyon
{
    class ttf_engine;

    class font : public sdl_object<TTF_Font, ::TTF_CloseFont>
    {
        friend class ttf_engine;

      public:
      private:

        font(const char* path, lyo::u8 size) noexcept;
    };
}  // namespace halcyon