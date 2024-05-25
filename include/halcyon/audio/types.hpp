#pragma once

#include <SDL_audio.h>

#include <halcyon/types/numeric.hpp>

namespace hal
{
    namespace audio
    {
        enum class format : u16
        {
            i8  = AUDIO_S8,
            i16 = AUDIO_S16SYS,
            i32 = AUDIO_S32SYS,

            u8  = AUDIO_U8,
            u16 = AUDIO_U16SYS,

            f32 = AUDIO_F32SYS
        };

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