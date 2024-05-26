#pragma once

#include <SDL_audio.h>

#include <halcyon/types/numeric.hpp>

namespace hal
{
    namespace audio
    {
        using freq_t = u32;

        // Formats in which audio data is stored.
        enum class format : u16
        {
            i8 = AUDIO_S8,

            i16     = AUDIO_S16SYS,
            i16_lsb = AUDIO_S16LSB,
            i16_msb = AUDIO_S16MSB,

            i32     = AUDIO_S32SYS,
            i32_lsb = AUDIO_S32LSB,
            i32_msb = AUDIO_S32MSB,

            u8 = AUDIO_U8,

            u16     = AUDIO_U16SYS,
            u16_lsb = AUDIO_U16LSB,
            u16_msb = AUDIO_U16MSB,

            f32     = AUDIO_F32SYS,
            f32_lsb = AUDIO_F32LSB,
            f32_msb = AUDIO_F32MSB
        };

        // Which changes to allow if the desired audio spec cannot be met.
        enum class change : u8
        {
            frequency = SDL_AUDIO_ALLOW_FREQUENCY_CHANGE,
            format    = SDL_AUDIO_ALLOW_FORMAT_CHANGE,
            channels  = SDL_AUDIO_ALLOW_CHANNELS_CHANGE,
            samples   = SDL_AUDIO_ALLOW_SAMPLES_CHANGE,
            any       = SDL_AUDIO_ALLOW_ANY_CHANGE
        };
    }
}