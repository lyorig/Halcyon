#pragma once

#include <SDL_audio.h>

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/utility/pass_key.hpp>

namespace hal
{
    namespace builder
    {
        class audio_device;
    }

    namespace audio
    {
        struct spec
        {
        };

        class device
        {
        public:
            device(pass_key<builder::audio_device>);

            ~device();

        private:
            SDL_AudioDeviceID m_id;
        };
    }
}