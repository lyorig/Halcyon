#pragma once

#include <SDL2/SDL_mixer.h>

#include <halcyon/types/tags.hpp>
#include <lyo/pass_key.hpp>
#include <lyo/timer.hpp>

#include "sdl_object.hpp"

namespace hal
{
    class mixer;

    class music : public sdl_object<Mix_Music, &::Mix_FreeMusic>
    {
    public:
        music(lyo::pass_key<mixer>);

        void play(const char* path, lyo::u16 loops = 0);
        void play(const char* path, infinite_loop_tag);

        void pause();
        void resume();

        bool playing() const;

        lyo::u8  volume() const;
        lyo::f64 position() const;
        lyo::f64 duration() const;

        void set_volume(lyo::u8 volume) const;
        void jump(double time);

    private:
        void internal_play(const char* path, int loops);

        lyo::precise_stopwatch m_timer;
    };
} // namespace hal