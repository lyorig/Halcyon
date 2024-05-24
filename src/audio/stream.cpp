#include <halcyon/audio/stream.hpp>

using namespace hal;

audio::stream::stream(format src_fmt, u8 src_channels, i32 src_rate, format dst_fmt, u8 dst_channels, i32 dst_rate)
    : raii_object { ::SDL_NewAudioStream(static_cast<SDL_AudioFormat>(src_fmt), src_channels, src_rate, static_cast<SDL_AudioFormat>(dst_fmt), dst_channels, dst_rate) }
{
}

i32 audio::stream::available() const
{
    return static_cast<bool>(::SDL_AudioStreamAvailable(get()));
}

void audio::stream::flush()
{
    HAL_ASSERT_VITAL(::SDL_AudioStreamFlush(get()) == 0, debug::last_error());
}

void audio::stream::clear()
{
    ::SDL_AudioStreamClear(get());
}

void audio::stream::put(std::span<std::byte> data)
{
    HAL_ASSERT_VITAL(::SDL_AudioStreamPut(get(), data.data(), static_cast<int>(data.size())) == 0, debug::last_error());
}