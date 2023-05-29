#pragma once

#include <unordered_map>

#include "components/chunk.hpp"
#include "components/music.hpp"
#include "internal/subsystem.hpp"

namespace halcyon
{
    class engine;

    class mixer
    {
      public:

        enum music_types : lyo::u8
        {
            none = 0,
            flac = MIX_INIT_FLAC,
            mod  = MIX_INIT_MOD,
            mp3  = MIX_INIT_MP3,
            ogg  = MIX_INIT_OGG,
            mid  = MIX_INIT_MID,
            opus = MIX_INIT_OPUS
        };

        mixer(engine& engine, int music_types, lyo::u16 freq, lyo::u8 channels) noexcept;
        ~mixer();

        chunk load_sfx(const char* path) const noexcept;

        [[no_unique_address]] subsystem<audio> m_subsys;

        /* Warning - only a single Mix_Music instance is allowed globally! */
        halcyon::music music;

      private:
    };
}  // namespace halcyon