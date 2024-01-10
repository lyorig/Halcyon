#pragma once

#include <SDL2/SDL_ttf.h>

#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>
#include <lyoSTL/pass_key.hpp>
#include <string_view>

#include <halcyon/components/SDL_object.hpp>

namespace hal
{
    class ttf_engine;
    class surface;

    LYOSTL_TAG(wrapped);

    class font : public sdl::object<TTF_Font, &::TTF_CloseFont>
    {
    public:
        font(const char* path, lyo::u8 size, lyo::pass_key<ttf_engine>);

        // Render a one-line string.
        surface render(const std::string_view& text, color color = color::white) const;

        // Render a multi-line string.
        surface render(wrapped_tag, const std::string_view& text, color color = color::white) const;

        // When sizing text, it's important to know that only the horizontal
        // size remains identical to the resulting render's size. The vertical
        // size is around 10% smaller.
        pixel_point size_text(const std::string_view& text) const;

        pixel_t height() const;
        pixel_t skip() const;
    };
}