#include "music.hpp"

using namespace hal;

void music::play() const noexcept
{
    HAL_DEBUG_VERIFY(m_object != nullptr, ::Mix_GetError());

    ::Mix_ResumeMusic();
}

void music::play(const char* path, lyo::u16 loops) noexcept
{
    m_object = ::Mix_LoadMUS(path);

    HAL_DEBUG_VERIFY(m_object != nullptr, "Tried to play null music");

    HAL_DEBUG_VERIFY(::Mix_PlayMusic(m_object, loops) == 0, ::Mix_GetError());
}

void music::pause() const noexcept
{
    HAL_DEBUG_VERIFY(m_object != nullptr, "Tried to pause null music");

    ::Mix_PauseMusic();
}

void music::fade_in(double time, lyo::u16 loops) const noexcept
{
    HAL_DEBUG_VERIFY(::Mix_FadeInMusic(m_object, loops, time * 1000.0) == 0, ::Mix_GetError());
}

void music::fade_out(double time) const noexcept
{
    HAL_DEBUG_VERIFY(::Mix_FadeOutMusic(time * 1000.0) == 0, ::Mix_GetError());
}

lyo::u8 music::volume() const noexcept
{
    const auto ret { ::Mix_VolumeMusic(-1) };

    HAL_DEBUG_VERIFY(ret == 0, ::Mix_GetError());

    return static_cast<lyo::u8>(ret);
}

lyo::f64 music::position() const noexcept
{
    return m_timer();
}

void music::set_volume(lyo::u8 volume) const noexcept
{
    HAL_DEBUG_VERIFY(m_object != nullptr, ::Mix_GetError());

    ::Mix_VolumeMusic(volume);
}

void music::jump(double time) noexcept
{
    ::Mix_RewindMusic();

    HAL_DEBUG_VERIFY(::Mix_SetMusicPosition(time) == 0, ::Mix_GetError());

    m_timer = time;
}