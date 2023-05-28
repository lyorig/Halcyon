#include "mixer.hpp"

using namespace halcyon;

mixer::mixer(engine& engine, int music_types, lyo::u16 freq, lyo::u8 channels) noexcept
{
    HALCYON_VERIFY(::Mix_Init(music_types) == music_types, "SDL_mixer initialization failed");
    HALCYON_VERIFY(::Mix_OpenAudio(freq, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) == 0, "Couldn't open audio device");
    HALCYON_VERIFY(::Mix_AllocateChannels(channels) == channels, "Couldn't allocate channels");

    HALCYON_PRINT(debug::info, "Initialized sound manager with ", ::Mix_AllocateChannels(-1), " channels");
}

mixer::~mixer()
{
    ::Mix_CloseAudio();
    ::Mix_Quit();
}

chunk mixer::load_sfx(const char* path) const noexcept
{
    return path;
}