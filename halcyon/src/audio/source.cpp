#include <halcyon/audio/source.hpp>

using namespace hal::audio;

source::source()
{
    HAL_AL_CALL(::alGenSources, 1, &m_id);
    HAL_PRINT("Generated source with ID ", m_id);
}

source::~source()
{
    set<buffer_id>(AL_NONE);
    HAL_AL_CALL(::alDeleteSources, 1, &m_id);
}

void source::play()
{
    HAL_AL_CALL(::alSourcePlay, m_id);
}

void source::pause()
{
    HAL_AL_CALL(::alSourcePause, m_id);
}

void source::rewind()
{
    HAL_AL_CALL(::alSourceRewind, m_id);
}

bool source::playing()
{
    return get<source_state>() == AL_PLAYING;
}

uint_t source::id() const { return m_id; }