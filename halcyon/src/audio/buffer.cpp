#include <halcyon/audio/buffer.hpp>

using namespace hal::audio;

buffer::buffer()
{
    HAL_AL_CALL(::alGenBuffers, 1, &m_id);
    HAL_PRINT("Generated buffer with ID ", m_id);
}

buffer::buffer(std::span<const char> data, format fmt, ssize_t sample_rate, lyo::pass_key<decoder>)
    : buffer {}
{
    HAL_AL_CALL(::alBufferData, m_id, fmt, data.data(), data.size(), sample_rate);
}

buffer::~buffer()
{
    HAL_AL_CALL(::alDeleteBuffers, 1, &m_id);
}

uint_t buffer::id() const { return m_id; }