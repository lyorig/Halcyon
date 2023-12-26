#pragma once

#include <SDL2/SDL_mixer.h>

#include <halcyon/types/tags.hpp>
#include <lyo/pass_key.hpp>

#include <halcyon/components/sdl_object.hpp>

namespace hal
{
    class engine;
    class mixer;

    class chunk : public sdl_object<Mix_Chunk, &::Mix_FreeChunk>
    {
    public:
        enum class quality : lyo::u16
        {
            low = 1024,
            medium = 2048,
            high = 4096
        };

        chunk(const char* path, lyo::pass_key<mixer>);

        // If you use these on an rvalue, that's on you, buddy.
        void play(lyo::u16 loops = 0) const;
        void play(infinite_loop_tag) const;

    private:
        void internal_play(int loops) const;
    };
} // namespace hal