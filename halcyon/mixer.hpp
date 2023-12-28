#pragma once

#include <halcyon/components/chunk.hpp>
#include <halcyon/components/music.hpp>
#include <halcyon/internal/subsystem.hpp>

namespace hal
{
    class engine;
    class chunk;

    // A mixer, which can play music and load sound effects.
    // Make sure it outlives these, as chunk destructors require
    // the various mixer libraries to be initialized.
    class mixer
    {
    public:
        using system = audio;

        mixer(const system& audio);
        mixer(const system& audio, lyo::u32 freq, lyo::u8 channels, chunk::quality qual);

        chunk load_sfx(const char* path) &;

    private:
        LYO_MAYBE_EMPTY class init
        {
        public:
            init(lyo::u32 freq, lyo::u8 channels, chunk::quality qual);
            ~init();
        } m_init;

    public:
        class music music;
    };
} // namespace hal