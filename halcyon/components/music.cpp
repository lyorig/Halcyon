#include "music.hpp"

using namespace halcyon;

void music::play() noexcept
{
    HALCYON_VERIFY(m_object != nullptr, "Tried to play null music");

    ::Mix_ResumeMusic();
}

void music::play(const char* path, lyo::u16 loops) noexcept
{
    m_object = ::Mix_LoadMUS(path);

    HALCYON_VERIFY(m_object != nullptr, "Tried to pause null music");

    HALCYON_VERIFY(::Mix_PlayMusic(m_object, loops) == 0, "Couldn't play music");
}

void music::pause() noexcept
{
    HALCYON_VERIFY(m_object != nullptr, "Tried pause null music");

    ::Mix_PauseMusic();
}

void music::fade_in(double time, lyo::u16 loops) const noexcept
{
    HALCYON_VERIFY(::Mix_FadeInMusic(m_object, loops, time * 1000.0) == 0, "Couldn't fade in music");
}

void music::fade_out(double time) const noexcept
{
    HALCYON_VERIFY(::Mix_FadeOutMusic(time * 1000.0) == 0, "Couldn't fade out music");
}

lyo::u8 music::volume() const noexcept
{
    return static_cast<lyo::u8>(::Mix_VolumeMusic(-1));
}

void music::set_volume(lyo::u8 volume) noexcept
{
    HALCYON_VERIFY(m_object != nullptr, "Tried to set volume of null music");

    ::Mix_VolumeMusic(volume);
}

void music::jump(double time) noexcept
{
    ::Mix_RewindMusic();

    HALCYON_VERIFY(::Mix_SetMusicPosition(time) == 0, "Couldn't set music position");
}