#include <AL++/buffer.hpp>
#include <AL++/source.hpp>

using namespace alpp;

source::source()
{
    ALPP_AL_CALL(::alGenSources, 1, &m_id);
    HAL_PRINT("Generated source with ID ", m_id);
}

source::~source()
{
    ALPP_AL_CALL(::alDeleteSources, 1, &m_id);
}

void source::attach(const buffer& buf)
{
    set<buffer_id>(buf.id());
}

void source::play()
{
    ALPP_AL_CALL(::alSourcePlay, m_id);
    HAL_PRINT("Playing source ", m_id, " (buffer ", get<buffer_id>(), ')');
}

void source::pause()
{
    ALPP_AL_CALL(::alSourcePause, m_id);
}

void source::rewind()
{
    ALPP_AL_CALL(::alSourceRewind, m_id);
}

bool source::playing() const
{
    return get<source_state>() == AL_PLAYING;
}

bool source::valid() const
{
    return ALPP_AL_CALL(::alIsSource, m_id);
}