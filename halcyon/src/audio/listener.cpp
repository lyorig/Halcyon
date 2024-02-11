#include <halcyon/audio/listener.hpp>

using namespace hal::audio;

listener::listener()
    : m_dev { ::alcOpenDevice(nullptr) }
    , m_ctx { ::alcCreateContext(m_dev, nullptr) }
{
    // The only error states for the c-tor functions are returning NULL.
    HAL_ASSERT(m_dev != nullptr, "Couldn't open audio device");
    HAL_ASSERT(m_ctx != nullptr, "Couldn't create audio context");

    HAL_ALC_CALL(m_dev, ::alcMakeContextCurrent, m_ctx);

    HAL_PRINT(hal::debug::load, "Audio device opened at ", m_dev, ", context at ", m_ctx);
}

listener::~listener()
{
    HAL_ALC_CALL(m_dev, ::alcMakeContextCurrent, nullptr);
    HAL_ALC_CALL(m_dev, ::alcDestroyContext, m_ctx);
    HAL_ALC_CALL(m_dev, ::alcCloseDevice, m_dev);
}