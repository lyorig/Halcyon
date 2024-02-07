#include <halcyon/audio/listener.hpp>

using namespace hal::audio;

listener::listener()
    : m_dev { ::alcOpenDevice(nullptr) }
    , m_ctx { ::alcCreateContext(m_dev, nullptr) }
{
    // The only error states for the c-tor functions are returning NULL.
    HAL_ASSERT(m_dev != nullptr, "Couldn't open audio device");
    HAL_ASSERT(m_ctx != nullptr, "Couldn't create audio context");

    HAL_ASSERT_VITAL(::alcMakeContextCurrent(m_ctx) == AL_TRUE, "Couldn't set active context");

    HAL_PRINT(hal::debug::load, "Audio device opened at ", m_dev, ", context at ", m_ctx);
}

listener::~listener()
{
    HAL_ASSERT_VITAL(::alcMakeContextCurrent(nullptr) == AL_TRUE, "Couldn't detach context");
    ::alcDestroyContext(m_ctx);
    HAL_ASSERT_VITAL(::alcCloseDevice(m_dev) == AL_TRUE, "Couldn't close device");
}