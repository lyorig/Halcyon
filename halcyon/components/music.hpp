#pragma once

#include <SDL2/SDL_mixer.h>

#include <lyo/timer.hpp>

#include "sdl_object.hpp"

namespace halcyon
{
    class mixer;

    class music : public sdl_object<Mix_Music, ::Mix_FreeMusic>
    {
        friend class mixer;

      public:

        void play() const noexcept;
        void play(const char* path, lyo::u16 loops = 0) noexcept;

        void pause() const noexcept;

        void fade_in(double time, lyo::u16 loops = 0) const noexcept;
        void fade_out(double time) const noexcept;

        lyo::u8 volume() const noexcept;

        void set_volume(lyo::u8 volume) const noexcept;
        void jump(double time) const noexcept;

      private:

        music() = default;

        lyo::precise_timer m_timer;
    };
}  // namespace halcyon