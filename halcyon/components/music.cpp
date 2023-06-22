#include "music.hpp"

#include <functional>

using namespace hal;

music::music(lyo::pass_key<mixer>) noexcept
{
    // TODO: Mix_HookMusicFinished to reset the timer when the music finishes.
}

void music::play(const char* path, lyo::u16 loops) noexcept
{
    this->reassign(::Mix_LoadMUS(path));

    HAL_ASSERT(::Mix_PlayMusic(m_object, loops) == 0, ::Mix_GetError());

    HAL_PRINT(severity::info, "Playing ", path);

    m_timer.reset();
}

void music::pause() noexcept
{
    HAL_CHECK(m_object != nullptr, "Tried to pause null music");

    ::Mix_PauseMusic();

    m_timer.pause();
}

void music::resume() noexcept
{
    HAL_ASSERT(m_object != nullptr, "Tried to resume null music");

    ::Mix_ResumeMusic();

    m_timer.resume();
}

void music::fade_in(const char* path, double time, lyo::u16 loops) noexcept
{
    this->reassign(::Mix_LoadMUS(path));

    HAL_ASSERT(::Mix_FadeInMusic(m_object, loops, time * 1000.0) == 0, ::Mix_GetError());

    HAL_PRINT(severity::info, "Fading in ", path);

    m_timer.reset();
}

void music::fade_out(double time) const noexcept
{
    if (::Mix_FadingMusic() != MIX_FADING_OUT)
        HAL_ASSERT(::Mix_FadeOutMusic(time * 1000.0) != 0, ::Mix_GetError());
}

bool music::playing() const noexcept
{
    return static_cast<bool>(::Mix_PlayingMusic());
}

lyo::u8 music::volume() const noexcept
{
    const auto ret { ::Mix_VolumeMusic(-1) };

    HAL_CHECK(ret == 0, ::Mix_GetError());

    return static_cast<lyo::u8>(ret);
}

lyo::f64 music::position() const noexcept
{
    return this->playing() ? m_timer() : 0.0;
}

void music::set_volume(lyo::u8 volume) const noexcept
{
    HAL_CHECK(m_object != nullptr, "Tried to set volume of null music");

    ::Mix_VolumeMusic(volume);
}

void music::jump(double time) noexcept
{
    ::Mix_RewindMusic();

    HAL_ASSERT(::Mix_SetMusicPosition(time) == 0, ::Mix_GetError());

    m_timer = time;
}