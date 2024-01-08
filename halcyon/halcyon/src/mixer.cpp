#include <halcyon/mixer.hpp>

#include <SDL2/SDL_mixer.h>

#include <halcyon/components/chunk.hpp>
#include <halcyon/debug.hpp>

using namespace hal;

mixer::init::init(lyo::u32 freq, lyo::u8 channels, chunk::quality qual)
{
    constexpr int types = MIX_INIT_MP3 | MIX_INIT_OGG;

    HAL_DEBUG_ASSERT_VITAL(::Mix_Init(types) == types, ::Mix_GetError());
    HAL_DEBUG_ASSERT_VITAL(::Mix_OpenAudio(freq, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, static_cast<int>(qual)) == 0, ::Mix_GetError());
    HAL_DEBUG_ASSERT_VITAL(::Mix_AllocateChannels(channels) == channels, ::Mix_GetError());

    HAL_DEBUG_PRINT(debug::init, "Initialized mixer with ", freq, " Hz, ", static_cast<lyo::u32>(channels), " channels and ", static_cast<lyo::u32>(qual), " quality");
}

mixer::init::~init()
{
    ::Mix_CloseAudio();
    ::Mix_Quit();
}

mixer::mixer(audio& sys)
    : mixer { sys, 44100, 8, chunk::quality::medium } // Sane defaults, I guess?
{
}

mixer::mixer(audio& sys [[maybe_unused]], lyo::u32 freq, lyo::u8 channels, chunk::quality qual)
    : m_init { freq, channels, qual }
    , music { {} }

{
}

chunk mixer::load_sfx(const char* path) &
{
    return { path, {} };
}