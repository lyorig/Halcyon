#pragma once

#include <halcyon/audio/get_set.hpp>

namespace hal::audio
{
    // This represents a listener. Only one can exist, so do not
    // create this class multiple times.
    class listener : get_set<listener, ::alGetListeneri, ::alGetListenerf, ::alGetListener3i, ::alGetListener3f, ::alListeneri, ::alListenerf, ::alListener3i, ::alListener3f>
    {
    public:
        static consteval auto properties()
        {
            return std::array { gain, position, velocity };
        }

        listener();
        ~listener();

        template <property P>
        prop_t<P> get()
        {
            return get_set::get<P>();
        }

        template <property P>
        void set(const prop_t<P>& value)
        {
            get_set::set<P>(value);
        }

    private:
        ALCdevice*  m_dev;
        ALCcontext* m_ctx;
    };
}