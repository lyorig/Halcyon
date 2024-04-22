#pragma once

#include <string_view>

#include <SDL2/SDL_ttf.h>

#include <halcyon/internal/accessor.hpp>
#include <halcyon/surface.hpp>

// ttf.hpp:
// SDL_ttf wrappers for font loading and text rendering.

namespace hal::ttf
{
    class font;

    // A class that makes sure everything TTF-related is loaded and
    // ready to use. This includes not only loading fonts, but also
    // their features - for example, font::render() will fail if a
    // TTF engine doesn't exist. TL;DR: Ensure that this object outlives
    // all fonts. This includes destructors!
    class context
    {
    public:
        // Initialize the TTF context.
        context();

        context(const context&) = delete;
        context(context&&)      = delete;

        ~context();

        // Font loading function.
        [[nodiscard]] font load(accessor data, u8 pt) &;

        static bool initialized();
    };

    static_assert(std::is_empty_v<context>);

    class font : public sdl::object<TTF_Font, &::TTF_CloseFont>
    {
    public:
        // [private] Fonts are loaded with ttf::context::load().
        font(TTF_Font* ptr, pass_key<context>);

        // Debug destructor to check whether the TTF context still exists.
        ~font();

        // Convenience text rendering function.
        [[nodiscard]] surface render(std::string_view text, color color = palette::white) const;

        // When sizing text, it's important to know that the vertical size
        // doesn't necessarily have to match that of the rendered surface.
        pixel_point size_text(const std::string_view& text) const;

        pixel_t height() const;
        pixel_t skip() const;

        std::string_view family() const;
        std::string_view style() const;
    };

    // Ensure calling debug::last_error() gives accurate information.
    static_assert(::TTF_GetError == ::SDL_GetError);
}