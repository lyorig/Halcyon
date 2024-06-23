#include <halcyon/audio/stream.hpp>

using namespace hal;

// Const audio stream view.

using cv = view<const audio::stream>;

i32 cv::available() const
{
    return static_cast<bool>(::SDL_AudioStreamAvailable(get()));
}

// Audio stream view.

using v = view<audio::stream>;

void v::flush()
{
    HAL_ASSERT_VITAL(::SDL_AudioStreamFlush(get()) == 0, debug::last_error());
}

void v::clear()
{
    ::SDL_AudioStreamClear(get());
}

void v::put(std::span<const std::byte> data)
{
    HAL_ASSERT_VITAL(::SDL_AudioStreamPut(get(), data.data(), static_cast<int>(data.size_bytes())) == 0, debug::last_error());
}

audio::stream::stream(proxy::audio&, config src, config dst)
    : raii_object { ::SDL_NewAudioStream(static_cast<SDL_AudioFormat>(src.fmt), src.channels, src.rate, static_cast<SDL_AudioFormat>(dst.fmt), dst.channels, dst.rate) }
{
}
