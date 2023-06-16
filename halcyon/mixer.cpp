#include "mixer.hpp"

#include <SDL2/SDL_mixer.h>

#include "debug.hpp"

using namespace hal;

mixer::init::init(lyo::u32 freq, lyo::u8 channels, chunk_quality qual) noexcept
{
    constexpr int types = MIX_INIT_MP3 | MIX_INIT_OGG;

    HAL_DEBUG_VERIFY(::Mix_Init(types) == types, ::Mix_GetError());
    HAL_DEBUG_VERIFY(::Mix_OpenAudio(freq, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, static_cast<int>(qual)) == 0, ::Mix_GetError());
    HAL_DEBUG_VERIFY(::Mix_AllocateChannels(channels) == channels, ::Mix_GetError());
}

mixer::init::~init() noexcept
{
    ::Mix_CloseAudio();
    ::Mix_Quit();
}

mixer::mixer(engine& eng) :
    m_init { 48000, 16, high }
{
}

mixer::mixer(engine& engine, lyo::u32 freq, lyo::u8 channels, chunk_quality qual) noexcept :
    m_init { freq, channels, qual }
{
}

chunk mixer::load_sfx(const char* path) noexcept
{
    return path;
}