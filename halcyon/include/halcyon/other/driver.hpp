#pragma once

#include <lyo/types.hpp>
#include <string_view>

namespace hal
{
    namespace driver
    {
        using index_t = lyo::u8;

        // Get the amount of available video drivers.
        index_t amount();

        // Get the name of the current video driver.
        std::string_view name();

        // Get the name of the video driver at [idx].
        std::string_view name(index_t idx);
    };
}