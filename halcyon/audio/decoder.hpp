#pragma once

#include <halcyon/audio/buffer.hpp>

namespace hal::audio
{
    class decoder
    {
    public:
        static buffer wav(const std::string_view& path);
    };
}