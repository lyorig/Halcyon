#pragma once

#include <halcyon/audio/base.hpp>
#include <lyo/pass_key.hpp>
#include <span>

namespace hal::audio
{
    class decoder;

    // A buffer filled with audio data. Can be constructed
    // via decoder classes.
    class buffer : object<::alGetBufferi, ::alGetBufferf, ::alGetBuffer3i, ::alGetBuffer3f, ::alBufferi, ::alBufferf, ::alBuffer3i, ::alBuffer3f>
    {
    public:
        buffer();
        buffer(std::span<const char> data, format fmt, ssize_t sample_rate, lyo::pass_key<decoder>);
        ~buffer();

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

        uint_t id() const;

    private:
        uint_t m_id;
    };
}