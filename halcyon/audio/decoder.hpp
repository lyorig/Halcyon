#pragma once

#include <halcyon/audio/buffer.hpp>
#include <halcyon/audio/get_set.hpp>
namespace hal::audio
{
    class decoder
    {
    public:
        static buffer wav(const std::string_view& path);
    };
}