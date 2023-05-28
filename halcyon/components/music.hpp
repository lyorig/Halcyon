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

        void play() noexcept;
        void play(const char* path, int loops = INT32_MAX) noexcept;

        void pause() noexcept;

        void set_volume(lyo::u8 volume) noexcept;
        void jump(double time) noexcept;

      private:

        music() = default;

        lyo::precise_timer m_timer;
    };
}  // namespace halcyon