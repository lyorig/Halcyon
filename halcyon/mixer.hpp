#pragma once

#include "components/music.hpp"
#include "internal/subsystem.hpp"

namespace hal
{
    // Loop music/SFX as many times as possible (~65K times).
    constexpr lyo::i16 infinite_loop { -1 };

    class engine;
    class chunk;

    class mixer
    {
      public:

        static music mus;  // Default-constructed, doesn't need prior subsystem initialization

        enum chunk_quality : lyo::u16
        {
            low    = 1024,
            medium = 2048,
            high   = 4096
        };

        mixer(engine& eng);
        mixer(engine& eng, lyo::u32 freq, lyo::u8 channels, chunk_quality qual) noexcept;

        chunk load_sfx(const char* path) const noexcept &;

      private:

        MAYBE_EMPTY subsystem<audio> m_subsys;

        MAYBE_EMPTY class init
        {
          public:

            init(lyo::u32 freq, lyo::u8 channels, chunk_quality qual) noexcept;
            ~init();
        } m_init;
    };
}  // namespace hal