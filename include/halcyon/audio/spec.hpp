#pragma once

#include <halcyon/audio/types.hpp>

#include <halcyon/utility/pass_key.hpp>

namespace hal
{
    namespace proxy
    {
        class audio_outputs;
        class audio_inputs;
    }

    namespace audio
    {
        class spec;

        namespace builder
        {
            class device;
        }

        class spec;

        namespace sdl
        {
            class spec : private SDL_AudioSpec
            {
            public:
                spec() = default;

                // Create your own spec.
                spec(freq_t samples_per_second, audio::format fmt, u8 channels, u16 buffer_size_in_frames);

                // Decompress a native spec.
                spec(const audio::spec& src);

                freq_t        hz() const;
                audio::format format() const;
                u8            channels() const;
                u16           buffer_size() const;

                SDL_AudioSpec* get(pass_key<builder::device>);
                SDL_AudioSpec* get(pass_key<proxy::audio_outputs>);
                SDL_AudioSpec* get(pass_key<proxy::audio_inputs>);

                friend std::ostream& operator<<(std::ostream& str, const spec& s);
            };

            static_assert(sizeof(spec) == sizeof(SDL_AudioSpec));
        }

        class spec
        {
        public:
            spec() = default;

            // Configure your own audio spec.
            spec(freq_t samples_per_second, audio::format fmt, u8 channels, u16 buffer_size_in_frames);

            // Compress a native audio spec.
            spec(const sdl::spec& src);

            freq_t hz;
            format fmt;
            u8     channels;
            u16    buffer_size_in_frames;
        };
    }
}