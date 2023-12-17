#include "music.hpp"

using namespace hal;

music::music(lyo::pass_key<mixer>)
{
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
    ::Mix_PauseMusic();
}

void music::resume()
{
    ::Mix_ResumeMusic();
}

bool music::playing() const
{
    return bool(::Mix_PlayingMusic());
}

lyo::u8 music::volume() const
{
    return static_cast<lyo::u8>(::Mix_VolumeMusic(-1));
}

// Bit of a bodge.
lyo::f64 music::position() const
{
    const auto ret = ::Mix_GetMusicPosition(this->ptr());

    HAL_DEBUG_CHECK(ret != -1.0, ::Mix_GetError());

    return ret;
}

lyo::f64 music::duration() const
{
    const auto ret = ::Mix_MusicDuration(this->ptr());

    HAL_DEBUG_CHECK(ret != -1.0, ::Mix_GetError());

    return ret;
}

void music::set_volume(lyo::u8 volume) const
{
    HAL_DEBUG_CHECK(m_object, "Tried to set volume of null music");

    ::Mix_VolumeMusic(volume);
}

void music::jump(lyo::f64 time)
{
    ::Mix_RewindMusic();

    HAL_DEBUG_ASSERT(::Mix_SetMusicPosition(time) == 0, ::Mix_GetError());
}

void music::internal_play(const char* path, int loops)
{
    sdl_object::operator=(::Mix_LoadMUS(path));

    HAL_DEBUG_ASSERT(::Mix_PlayMusic(m_object.get(), loops) == 0, ::Mix_GetError());
    HAL_DEBUG_PRINT(severity::load, "Loaded music ", path, " (appx. ", lyo::cast<lyo::u32>(this->duration()), "s)");
}