#pragma once

#include <span>

#include <halcyon/audio/types.hpp>

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/internal/subsystem.hpp>

#include <halcyon/utility/concepts.hpp>

namespace hal
{
    namespace detail
    {
        template <>
        class view_impl<SDL_AudioStream> : public view_base<SDL_AudioStream>
        {
            using view_base::view_base;
        };
    }

    namespace audio
    {
        struct config
        {
            format fmt;
            u8     channels;
            i32    rate;
        };

        class stream : public detail::raii_object<SDL_AudioStream, SDL_FreeAudioStream>
        {
        public:
            // Default constructor. Creates an invalid stream.
            stream() = default;

            stream(proxy::audio& sys, config src, config dst);

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