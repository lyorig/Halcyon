#pragma once

#include <SDL2/SDL_mixer.h>

#include <halcyon/types/tags.hpp>
#include <lyo/pass_key.hpp>
#include <lyo/timer.hpp>

#include <halcyon/components/sdl_object.hpp>

namespace hal
{
    class mixer;

    class music : public sdl_object<Mix_Music, &::Mix_FreeMusic>
    {
    public:
        music(lyo::pass_key<mixer>);

        music& load(const char* path);

        music& play(lyo::u16 loops = 0);
        music& play(infinite_loop_tag);

        music& fade_in(lyo::f64 time, lyo::u16 loops = 0);
        music& fade_in(lyo::f64 time, infinite_loop_tag);

        void fade_out(lyo::f64 time);

        // As everything audio-related happens in a separate thread,
        // music can sometimes begin unsynced. This function aims to
        // mitigate that by spinning until the track has been started.
        void sync();

        music& pause();
        music& resume();

        music& rewind();

        bool playing() const;

        lyo::u8  volume() const;
        lyo::f64 position() const;
        lyo::f64 duration() const;

        music& set_volume(lyo::u8 volume);
        music& jump(lyo::f64 time);

    private:
        music& internal_load(const char* path);
        music& internal_play(int loops);
        music& internal_fade(lyo::f64 time, int loops);
    };
} // namespace hal