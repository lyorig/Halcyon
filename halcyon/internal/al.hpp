#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <halcyon/types/audio.hpp>

// al.hpp:
// A single-file OpenAL wrapper.

namespace hal
{
    namespace audio
    {
        using pos = vec3<ALfloat>;

        class listener
        {
        public:
            listener()
                : m_dev { ::alcOpenDevice(nullptr) }
                , m_ctx { ::alcCreateContext(m_dev, nullptr) }
            {
                ::alcMakeContextCurrent(m_ctx);
            }

            void move(const pos& p)
            {
                ::alListener3f(AL_POSITION, p.x, p.y, p.z);
            }

            pos position() const
            {
                pos ret;
                ::alGetListener3f(AL_POSITION, &ret.x, &ret.y, &ret.z);
                return ret;
            }

            ~listener()
            {
                ::alcMakeContextCurrent(nullptr);
                ::alcDestroyContext(m_ctx);
                ::alcCloseDevice(m_dev);
            }

        private:
            ALCdevice*  m_dev;
            ALCcontext* m_ctx;
        };

        class source
        {
        public:
            source()
            {
                ::alGenSources(1, &m_id);
            }

            void play()
            {
                ::alSourcePlay(m_id);
            }

            void pause()
            {
                ::alSourcePause(m_id);
            }

            void rewind()
            {
                ::alSourceRewind(m_id);
            }

            void move(const pos& p)
            {
                ::alSource3f(m_id, AL_POSITION, p.x, p.y, p.x);
            }

            pos position() const
            {
                pos ret;
                ::alGetSource3f(m_id, AL_POSITION, &ret.x, &ret.y, &ret.z);
                return ret;
            }

            ~source()
            {
                ::alSourcei(m_id, AL_BUFFER, AL_NONE);
                ::alDeleteSources(1, &m_id);
            }

        private:
            ALuint m_id;
        };

        class buffer
        {
        public:
            buffer()
            {
                ::alGenBuffers(1, &m_id);
            }

            ~buffer()
            {
                ::alDeleteBuffers(1, &m_id);
            }

        private:
            ALuint m_id;
        };
    }
}