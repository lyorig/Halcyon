#pragma once

#include <SDL2/SDL_mixer.h>

#include <halcyon/types/tags.hpp>
#include <lyo/pass_key.hpp>
#include <lyo/timer.hpp>

#include "sdl_object.hpp"

namespace hal {
class mixer;

class music : public sdl_object<Mix_Music, &::Mix_FreeMusic> {
public:
    music(lyo::pass_key<mixer>) noexcept;

    void play(const char* path, lyo::u16 loops = 0) noexcept;
    void play(const char* path, infinite_loop_tag) noexcept;

    void pause() noexcept;
    void resume() noexcept;

    bool playing() const noexcept;

    lyo::u8 volume() const noexcept;
    lyo::f64 position() const noexcept;

    void set_volume(lyo::u8 volume) const noexcept;
    void jump(double time) noexcept;

private:
    void internal_play(const char* path, int loops) noexcept;

    lyo::precise_stopwatch m_timer;
};
} // namespace hal