#include "music.hpp"

#include <functional>

using namespace hal;

music::music(lyo::pass_key<mixer>)
{
    // TODO: Mix_HookMusicFinished to reset the timer when the music finishes.
}

void music::play(const char* path, lyo::u16 loops)
{
    this->internal_play(path, loops);
}

void music::play(const char* path, infinite_loop_tag)
{
    this->internal_play(path, -1);
}

void music::pause()
{
    HAL_DEBUG_CHECK(m_object, "Tried to pause null music");

    ::Mix_PauseMusic();

    m_timer.pause();
}

void music::resume()
{
    HAL_DEBUG_CHECK(m_object, "Tried to resume null music");

    ::Mix_ResumeMusic();

    m_timer.resume();
}

bool music::playing() const
{
    return static_cast<bool>(::Mix_PlayingMusic());
}

lyo::u8 music::volume() const
{
    return static_cast<lyo::u8>(::Mix_VolumeMusic(-1));
}

lyo::f64 music::position() const
{
    return this->playing() ? m_timer() : 0.0;
}

void music::set_volume(lyo::u8 volume) const
{
    HAL_DEBUG_CHECK(m_object, "Tried to set volume of null music");

    ::Mix_VolumeMusic(volume);
}

void music::jump(double time)
{
    ::Mix_RewindMusic();

    HAL_DEBUG_ASSERT(::Mix_SetMusicPosition(time) == 0, ::Mix_GetError());

    m_timer = time;
}

void music::internal_play(const char* path, int loops)
{
    this->reassign(::Mix_LoadMUS(path));

    HAL_DEBUG_ASSERT(::Mix_PlayMusic(m_object.get(), loops) == 0, ::Mix_GetError());

    HAL_DEBUG_PRINT(severity::load, "Loaded music ", path);

    m_timer.reset();
}