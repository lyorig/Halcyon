#pragma once

#include <AL++/buffer.hpp>
#include <AL++/get_set.hpp>
#include <string_view>

namespace alpp
{
    class decoder
    {
    public:
        static buffer wav(const std::string_view& path);
        static buffer mp3(const std::string_view& path);
    };
}