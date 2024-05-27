#pragma once

#include <span>

#include <halcyon/audio/types.hpp>

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/internal/subsystem.hpp>

#include <halcyon/utility/concepts.hpp>

namespace hal
{
    namespace audio
    {
        class stream : public detail::raii_object<SDL_AudioStream, SDL_FreeAudioStream>
        {
        public:
            stream(proxy::audio& sys, format src_fmt, u8 src_channels, i32 src_rate, format dst_fmt, u8 dst_channels, i32 dst_rate);

            i32 available() const;

            void flush();

            void clear();

            void put(std::span<const std::byte> data);

            template <meta::buffer T>
            i32 get_processed(T& buffer)
            {
                const int ret { ::SDL_AudioStreamGet(get(), std::data(buffer), std::size(buffer)) };

                HAL_ASSERT(ret != -1, debug::last_error());

                return ret;
            }
        };
    }
}