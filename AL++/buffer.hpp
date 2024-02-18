#pragma once

#include <AL++/object.hpp>
#include <lyo/pass_key.hpp>
#include <span>

namespace alpp
{
    class decoder;

    // A buffer filled with audio data. Can be constructed
    // via decoder classes.
    class buffer : public object<buffer, ::alGetBufferi, ::alGetBufferf, ::alGetBuffer3i, ::alGetBuffer3f, ::alBufferi, ::alBufferf, ::alBuffer3i, ::alBuffer3f>
    {
    public:
        static consteval auto properties()
        {
            return std::array { frequency, bit_depth, channels, size };
        }

        buffer();
        buffer(const void* data, std::size_t sz, format fmt, size_t sample_rate, lyo::pass_key<decoder>);
        ~buffer();
    };
}