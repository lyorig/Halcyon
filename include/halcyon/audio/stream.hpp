#pragma once

#include <span>

#include <halcyon/audio/types.hpp>

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/internal/subsystem.hpp>

namespace hal
{
    namespace audio
    {
        class stream : public detail::raii_object<SDL_AudioStream, SDL_FreeAudioStream>
        {
        public:
            stream(format src_fmt, u8 src_channels, i32 src_rate, format dst_fmt, u8 dst_channels, i32 dst_rate, pass_key<proxy::audio>);

            i32 available() const;

            void flush();

            void clear();

            void put(std::span<std::byte> data);

            template <typename T>
            i32 get_processed(std::span<T> buffer)
            {
                const int ret { ::SDL_AudioStreamGet(get(), buffer.data(), buffer.size_bytes()) };

                HAL_ASSERT(ret != -1, debug::last_error());

                return ret;
            }
        };
    }
}