#pragma once

#include <halcyon/audio/device.hpp>
#include <halcyon/audio/stream.hpp>

#include <halcyon/internal/subsystem.hpp>

namespace hal
{
    namespace proxy
    {
        class audio_outputs
        {
        public:
            audio_outputs(pass_key<audio>);

            hal::audio::device::id_t size() const;
            std::string_view         name(hal::audio::device::id_t idx) const;
        };

        class audio_inputs
        {
        public:
            audio_inputs(pass_key<audio>);

            hal::audio::device::id_t size() const;
            std::string_view         name(hal::audio::device::id_t idx) const;
        };
    }

    namespace detail
    {
        template <>
        class subsystem<system::audio>
        {
        public:
            subsystem(pass_key<hal::system::audio>);

            [[nodiscard]] audio::builder::device make_device() &;
            [[nodiscard]] audio::stream          make_stream(audio::format src_fmt, u8 src_channels, i32 src_rate, audio::format dst_fmt, u8 dst_channels, i32 dst_rate) &;

            HAL_NO_SIZE proxy::audio_outputs outputs;
            HAL_NO_SIZE proxy::audio_inputs inputs;
        };
    }
}