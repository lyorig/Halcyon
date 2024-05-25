#include <halcyon/audio.hpp>

using namespace hal;

proxy::audio_outputs::audio_outputs(pass_key<audio>)
{
}

audio::device::id_t proxy::audio_outputs::size() const
{
    return static_cast<hal::audio::device::id_t>(::SDL_GetNumAudioDevices(false));
}

std::string_view proxy::audio_outputs::name(hal::audio::device::id_t idx) const
{
    const char* ret { ::SDL_GetAudioDeviceName(idx, false) };

    HAL_ASSERT(ret != nullptr, debug::last_error());

    return ret;
}

proxy::audio_inputs::audio_inputs(pass_key<audio>)
{
}

audio::device::id_t proxy::audio_inputs::size() const
{
    return static_cast<hal::audio::device::id_t>(::SDL_GetNumAudioDevices(true));
}

std::string_view proxy::audio_inputs::name(hal::audio::device::id_t idx) const
{
    const char* ret { ::SDL_GetAudioDeviceName(idx, true) };

    HAL_ASSERT(ret != nullptr, debug::last_error());

    return ret;
}

proxy::audio::subsystem(pass_key<hal::system::audio>)
    : outputs { pass_key<proxy::audio> {} }
    , inputs { pass_key<proxy::audio> {} }
{
}

audio::builder::device proxy::audio::make_device() &
{
    return { pass_key<proxy::audio> {} };
}

audio::stream proxy::audio::make_stream(hal::audio::format src_fmt, u8 src_channels, i32 src_rate, hal::audio::format dst_fmt, u8 dst_channels, i32 dst_rate) &
{
    return { src_fmt, src_channels, src_rate, dst_fmt, dst_channels, dst_rate, pass_key<proxy::audio> {} };
}