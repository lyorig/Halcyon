#include "music.hpp"

#include <functional>

using namespace hal;

music::music(lyo::pass_key<mixer>) noexcept
{
    // TODO: Mix_HookMusicFinished to reset the timer when the music finishes.
}

void music::play(const char* path, loop_type loops) noexcept
{
    this->reassign(::Mix_LoadMUS(path));

    HAL_DEBUG_ASSERT(::Mix_PlayMusic(m_object, loops) == 0, ::Mix_GetError());

    HAL_DEBUG_PRINT(severity::load, "Loaded music ", path);

    m_timer.reset();
}

void music::pause() noexcept
{
    HAL_DEBUG_CHECK(m_object != nullptr, "Tried to pause null music");

    ::Mix_PauseMusic();

    m_timer.pause();
}

void music::resume() noexcept
{
    HAL_DEBUG_CHECK(m_object != nullptr, "Tried to resume null music");

    ::Mix_ResumeMusic();

    m_timer.resume();
}

bool music::playing() const noexcept
{
    return static_cast<bool>(::Mix_PlayingMusic());
}

lyo::u8 music::volume() const noexcept
{
    return static_cast<lyo::u8>(::Mix_VolumeMusic(-1));
}

lyo::f64 music::position() const noexcept
{
    return this->playing() ? m_timer() : 0.0;
}

void music::set_volume(lyo::u8 volume) const noexcept
{
    HAL_DEBUG_CHECK(m_object != nullptr, "Tried to set volume of null music");

    ::Mix_VolumeMusic(volume);
}

void music::jump(double time) noexcept
{
    ::Mix_RewindMusic();

    HAL_DEBUG_ASSERT(::Mix_SetMusicPosition(time) == 0, ::Mix_GetError());

    m_timer = time;
}