#include "mixer.hpp"

#include <SDL2/SDL_mixer.h>

#include "components/chunk.hpp"
#include "debug.hpp"

using namespace hal;

mixer::init::init(lyo::u32 freq, lyo::u8 channels, chunk_quality qual)
{
    constexpr int types = MIX_INIT_MP3 | MIX_INIT_OGG;

    HAL_DEBUG_ASSERT(::Mix_Init(types) == types, ::Mix_GetError());
    HAL_DEBUG_ASSERT(::Mix_OpenAudio(freq, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, static_cast<int>(qual)) == 0, ::Mix_GetError());
    HAL_DEBUG_ASSERT(::Mix_AllocateChannels(channels) == channels, ::Mix_GetError());

    HAL_DEBUG_PRINT(severity::init, "Initialized mixer with ", freq, " Hz, ", static_cast<lyo::u32>(channels), " channels and ", static_cast<lyo::u32>(qual), " quality");
}

mixer::init::~init()
{
    ::Mix_CloseAudio();
    ::Mix_Quit();
}

mixer::mixer(engine& eng)
    : mixer { eng, 48000, 8, chunk_quality::medium }
{
}

mixer::mixer(engine& engine, lyo::u32 freq, lyo::u8 channels, chunk_quality qual)
    : m_init { freq, channels, qual }
    , mus { {} }

{
}

chunk mixer::load_sfx(const char* path) &
{
    return { path, {} };
}