#pragma once

#include "components/font.hpp"

namespace hal
{
    class window;

    TAG_TYPE(rvalue_font);

    // A class that makes sure everything TTF-related is loaded and
    // ready to use. This includes not only loading fonts, but also
    // their features - for example, font::render() will fail if a
    // TTF engine doesn't exist.
    class ttf_engine
    {
      public:

        ttf_engine(window& wnd) noexcept;
        ~ttf_engine();

        // Beware: a big foot-gun here is creating an lvalue font from an rvalue TTF engine.
        // This WILL cause a crash, as the loader will de-initialize the TTF engine
        // before the font itself tries to do the same, causing a segmentation fault.

        // Create a font from an lvalue font loader. If you want to construct
        // it from (and use it) as an rvalue and have no intentions of any
        // further TTF operations, add the rvalue_font tag type.
        font load_font(const char* path, lyo::u8 size) & noexcept;

        // Give a pinky promise to this library's creator that you'll only
        // use the returned font as an rvalue and have absolutely no plans
        // to do something utterly stupid like assign it to a variable.
        font load_font(rvalue_font_tag, const char* path, lyo::u8 size) && noexcept;
    };
}  // namespace hal