#include <AL++/listener.hpp>
#include <AL++/state.hpp>

using namespace alpp;

listener::listener()
    : m_dev { ::alcOpenDevice(nullptr) }
    , m_ctx { ::alcCreateContext(m_dev, nullptr) }
{
    // The only error states for the c-tor functions are returning NULL.
    HAL_ASSERT(m_dev != nullptr, "Couldn't open audio device");
    HAL_ASSERT(m_ctx != nullptr, "Couldn't create audio context");

    ALPP_ALC_CALL(m_dev, ::alcMakeContextCurrent, m_ctx);

    HAL_PRINT(hal::debug::load, "Audio device opened at ", m_dev, ", context at ", m_ctx);
    HAL_PRINT("OpenAL v", state::query(state::version), " provided by ", state::query(state::vendor));
    HAL_PRINT("Input: ", query(input_device_name), ", output: ", query(output_device_name));
}

listener::~listener()
{
    ALPP_ALC_CALL(m_dev, ::alcMakeContextCurrent, nullptr);
    ALPP_ALC_CALL(m_dev, ::alcDestroyContext, m_ctx);
    ALPP_ALC_CALL(m_dev, ::alcCloseDevice, m_dev);
}

std::string_view listener::query(context_query what) const
{
    return ALPP_ALC_CALL(m_dev, ::alcGetString, m_dev, what);
}