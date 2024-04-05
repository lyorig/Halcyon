#pragma once

#include <SDL_ttf.h>
#include <halcyon/internal/accessor.hpp>
#include <halcyon/internal/sdl_object.hpp>
#include <halcyon/surface.hpp>
#include <string_view>

namespace hal
{
    // A class that makes sure everything TTF-related is loaded and
    // ready to use. This includes not only loading fonts, but also
    // their features - for example, font::render() will fail if a
    // TTF engine doesn't exist.
    // TL;DR: Ensure that this object outlives all fonts.

    class ttf_cleanup
    {
    public:
        ttf_cleanup();
        ~ttf_cleanup();

        static bool initialized();
    };

    class font : public sdl::object<TTF_Font, &::TTF_CloseFont>
    {
    public:
        font(accessor data, lyo::u8 size);

        surface render(const std::string_view& text, color color = color::white) const;

        // When sizing text, it's important to know that the vertical size
        // doesn't necessarily have to match that of the rendered surface.
        pixel_point size_text(const std::string_view& text) const;

        pixel_t height() const;
        pixel_t skip() const;

        std::string_view family() const;
        std::string_view style() const;
    };
} // namespace hal