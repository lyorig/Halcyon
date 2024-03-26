#include <AL++/buffer.hpp>

using namespace alpp;

buffer::buffer()
{
    ALPP_AL_CALL(::alGenBuffers, 1, &m_id);
    HAL_PRINT("Generated buffer with ID ", m_id);
}

buffer::buffer(const void* data, std::size_t sz, format fmt, size_t sample_rate, lyo::pass_key<decoder>)
    : buffer {}
{
    ALPP_AL_CALL(::alBufferData, m_id, fmt, data, sz, sample_rate);
}

buffer::~buffer()
{
    ALPP_AL_CALL(::alDeleteBuffers, 1, &m_id);
}