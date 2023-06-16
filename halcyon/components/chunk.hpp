#pragma once

#include <SDL2/SDL_mixer.h>

#include "sdl_object.hpp"

// TODO: Convert to OpenAL

namespace hal
{
    class engine;
    class mixer;

    class chunk : public sdl_object<Mix_Chunk, ::Mix_FreeChunk>
    {
        friend class mixer;

      public:

        void play() noexcept;

      private:

        chunk(const char* path) noexcept;
    };
}  // namespace hal