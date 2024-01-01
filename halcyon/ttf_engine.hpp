#pragma once

#include <halcyon/components/font.hpp>

namespace hal
{
    class video;

    // A class that makes sure everything TTF-related is loaded and
    // ready to use. This includes not only loading fonts, but also
    // their features - for example, font::render() will fail if a
    // TTF engine doesn't exist.
    class ttf_engine
    {
    public:
        ttf_engine(video& sys);
        ~ttf_engine();

        // Warning: If loading from an rvalue TTF engine, only
        // use the font as an rvalue. Do not assign it to a variable
        // or anything like that, because its usage after this class'
        // destructor runs is a one-way trip to seg-fault land.
        font load(const char* path, lyo::u8 size) const;
    };
} // namespace hal