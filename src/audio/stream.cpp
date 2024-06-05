#include <halcyon/audio/stream.hpp>

using namespace hal;

audio::stream::stream(proxy::audio&, config src, config dst)
    : raii_object { ::SDL_NewAudioStream(static_cast<SDL_AudioFormat>(src.fmt), src.channels, src.rate, static_cast<SDL_AudioFormat>(dst.fmt), dst.channels, dst.rate) }
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

void audio::stream::put(std::span<const std::byte> data)
{
    HAL_ASSERT_VITAL(::SDL_AudioStreamPut(get(), data.data(), static_cast<int>(data.size_bytes())) == 0, debug::last_error());
}