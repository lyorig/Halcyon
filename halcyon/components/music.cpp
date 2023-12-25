#include "music.hpp"
#include <lyo/cast.hpp>

using namespace hal;

music::music(lyo::pass_key<mixer>)
{
}

music& music::load(const char* path)
{
    return this->internal_load(path);
}

music& music::play(lyo::u16 loops)
{
    return this->internal_play(loops);
}

music& music::play(infinite_loop_tag)
{
    return this->internal_play(-1);
}

music& music::fade_in(lyo::f64 time, lyo::u16 loops)
{
    return this->internal_fade(time, loops);
}

music& music::fade_in(lyo::f64 time, infinite_loop_tag)
{
    return this->internal_fade(time, -1);
}

void music::fade_out(lyo::f64 time)
{
    // There is apparently no fail state here. It only returns
    // a non-zero value if it's already fading, but that's allowed.
    ::Mix_FadeOutMusic(lyo::cast<int>(time * 1000.0));
}

void music::sync()
{
    while (!this->playing())
        ;
}

music& music::pause()
{
    ::Mix_PauseMusic();
    return *this;
}

music& music::resume()
{
    ::Mix_ResumeMusic();
    return *this;
}

music& music::rewind()
{
    ::Mix_RewindMusic();
    return *this;
}

bool music::playing() const
{
    return bool(::Mix_PlayingMusic());
}

lyo::u8 music::volume() const
{
    return lyo::u8(::Mix_VolumeMusic(-1));
}

lyo::f64 music::position() const
{
    const auto ret = ::Mix_GetMusicPosition(this->ptr());

    HAL_DEBUG_ASSERT(ret != -1.0, ::Mix_GetError());

    return ret;
}

lyo::f64 music::duration() const
{
    const auto ret = ::Mix_MusicDuration(this->ptr());

    HAL_DEBUG_ASSERT(ret != -1.0, ::Mix_GetError());

    return ret;
}

music& music::set_volume(lyo::u8 volume)
{
    HAL_DEBUG_ASSERT(this->ptr() != nullptr, "Tried to set volume of null music");

    ::Mix_VolumeMusic(volume);

    return *this;
}

music& music::jump(lyo::f64 time)
{
    this->rewind();

    HAL_DEBUG_ASSERT_VITAL(::Mix_SetMusicPosition(time) == 0, ::Mix_GetError());

    return *this;
}

music& music::internal_load(const char* path)
{
    sdl_object::operator=(::Mix_LoadMUS(path));
    HAL_DEBUG_PRINT(debug::load, "Loaded music ", path, " (appx. ", lyo::cast<lyo::u32>(this->duration()), "s)");

    return *this;
}

music& music::internal_play(int loops)
{
    HAL_DEBUG_ASSERT_VITAL(::Mix_PlayMusic(this->ptr(), loops) == 0, ::Mix_GetError());
    return *this;
}

music& music::internal_fade(lyo::f64 time, int loops)
{
    HAL_DEBUG_ASSERT_VITAL(::Mix_FadeInMusic(this->ptr(), loops, lyo::cast<int>(time * 1000.0)) == 0, ::Mix_GetError());
    return *this;
}