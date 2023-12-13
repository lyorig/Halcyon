#pragma once

#include "components/music.hpp"
#include "internal/subsystem.hpp"

namespace hal
{
    class engine;
    class chunk;

    enum class chunk_quality : lyo::u16
    {
        low = 1024,
        medium = 2048,
        high = 4096
    };

    // A mixer, which can play music and load sound effects.
    // Make sure it outlives these, as chunk destructors require
    // the various mixer libraries to be initialized.
    class mixer
    {
    public:
        mixer(engine& eng);
        mixer(engine& eng, lyo::u32 freq, lyo::u8 channels, chunk_quality qual);

        chunk load_sfx(const char* path) &;

    private:
        MAYBE_EMPTY subsystem<subsys::audio> m_subsys;

        MAYBE_EMPTY class init
        {
        public:
            init(lyo::u32 freq, lyo::u8 channels, chunk_quality qual);
            ~init();
        } m_init;

    public:
        class music music;
    };
} // namespace hal