#pragma once

#include <SDL2/SDL_mixer.h>

#include <halcyon/types/tags.hpp>
#include <lyo/pass_key.hpp>

#include "sdl_object.hpp"

namespace hal
{
    class engine;
    class mixer;

    class chunk : public sdl_object<Mix_Chunk, &::Mix_FreeChunk>
    {
      public:

        chunk(const char* path, lyo::pass_key<mixer>) noexcept;

        void play(lyo::u16 loops = 0) & noexcept;
        void play(infinite_loop_t) & noexcept;

      private:

        void internal_play(int loops) noexcept;
    };
}  // namespace hal