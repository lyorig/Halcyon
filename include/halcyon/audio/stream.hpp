#pragma once

#include <span>

#include <SDL_audio.h>

#include <halcyon/internal/raii_object.hpp>

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

        class stream : public detail::raii_object<SDL_AudioStream, SDL_FreeAudioStream>
        {
        public:
            stream(format src_fmt, u8 src_channels, i32 src_rate, format dst_fmt, u8 dst_channels, i32 dst_rate);

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