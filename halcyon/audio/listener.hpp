#pragma once

#include <halcyon/audio/base.hpp>

namespace hal::audio
{
    // This represents a listener. Only one can exist, so do not
    // create this class multiple times.
    class listener : object<::alGetListeneri, ::alGetListenerf, ::alGetListener3i, ::alGetListener3f, ::alListeneri, ::alListenerf, ::alListener3i, ::alListener3f>
    {
    public:
        listener();
        ~listener();

        template <property P>
        prop_t<P> get()
        {
            return object::get<P>();
        }

        template <property P>
        void set(const prop_t<P>& value)
        {
            object::set<P>(value);
        }

    private:
        ALCdevice*  m_dev;
        ALCcontext* m_ctx;
    };
}