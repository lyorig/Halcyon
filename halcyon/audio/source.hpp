#pragma once

#include <halcyon/audio/base.hpp>

namespace hal::audio
{
    // A source point in 3D space from which the sound eminates.
    class source : object<::alGetSourcei, ::alGetSourcef, ::alGetSource3i, ::alGetSource3f, ::alSourcei, ::alSourcef, ::alSource3i, ::alSource3f>
    {
    public:
        source();
        ~source();

        template <property P>
        prop_t<P> get()
        {
            return object::get<P>(m_id);
        }

        template <property P>
        void set(const prop_t<P>& value)
        {
            return object::set<P>(value);
        }

        void play();
        void pause();
        void rewind();

        bool playing();

        uint_t id() const;

    private:
        uint_t m_id;
    };
}