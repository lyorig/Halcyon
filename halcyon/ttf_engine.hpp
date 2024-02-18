#pragma once

#include <halcyon/components/font.hpp>

namespace hal
{
    class video;

    // A class that makes sure everything TTF-related is loaded and
    // ready to use. This includes not only loading fonts, but also
    // their features - for example, font::render() will fail if a
    // TTF engine doesn't exist.
    // TL;DR: Ensure that this object outlives all fonts.

    class ttf_engine
    {
    public:
        ttf_engine();
        ~ttf_engine();

        static bool initialized();
    };
} // namespace hal