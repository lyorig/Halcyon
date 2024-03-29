#pragma once

#include <AL++/buffer.hpp>
#include <AL++/get_set.hpp>
#include <filesystem>

namespace alpp
{
    class decoder
    {
    public:
        static buffer wav(const std::filesystem::path& path);
    };
}