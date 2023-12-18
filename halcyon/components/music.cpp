#include "music.hpp"

using namespace hal;

music::music(lyo::pass_key<mixer>)
{
}

music& music::load(const char* path)
{
    this->internal_load(path);
    return *this;
}

void music::play(lyo::u16 loops)
{
    this->internal_play(loops);
}

void music::play(infinite_loop_tag)
{
    this->internal_play(-1);
}

void music::fade_in(lyo::f64 time, lyo::u16 loops)
{
    this->internal_fade(time, loops);
}

void music::fade_in(lyo::f64 time, infinite_loop_tag)
{
    this->internal_fade(time, -1);
}

void music::fade_out(lyo::f64 time)
{
    // There is apparently no fail state here. It only returns
    // a non-zero value if it's already fading, but that's allowed.
    ::Mix_FadeOutMusic(lyo::cast<int>(time * 1000.0));
}

void music::pause()
{
    ::Mix_PauseMusic();
}

void music::resume()
{
    ::Mix_ResumeMusic();
}

void music::rewind()
{
    ::Mix_RewindMusic();
}

bool music::playing() const
{
    return bool(::Mix_PlayingMusic());
}

lyo::u8 music::volume() const
{
    return static_cast<lyo::u8>(::Mix_VolumeMusic(-1));
}

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
    this->rewind();

    HAL_DEBUG_ASSERT(::Mix_SetMusicPosition(time) == 0, ::Mix_GetError());
}

void music::internal_load(const char* path)
{
    sdl_object::operator=(::Mix_LoadMUS(path));
    HAL_DEBUG_PRINT(severity::load, "Loaded music ", path, " (appx. ", lyo::cast<lyo::u32>(this->duration()), "s)");
}

void music::internal_play(int loops)
{
    HAL_DEBUG_ASSERT(::Mix_PlayMusic(m_object.get(), loops) == 0, ::Mix_GetError());
}

void music::internal_fade(lyo::f64 time, int loops)
{
    HAL_DEBUG_ASSERT(::Mix_FadeInMusic(this->ptr(), loops, lyo::cast<int>(time * 1000.0)) == 0, ::Mix_GetError());
}