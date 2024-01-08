#pragma once

#include <SDL2/SDL_ttf.h>

#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>
#include <string_view>

#include <halcyon/components/SDL_object.hpp>

namespace hal
{
    class ttf_engine;
    class surface;

    class font : public SDL::object<TTF_Font, &::TTF_CloseFont>
    {
    public:
        font(const char* path, lyo::u8 size, lyo::pass_key<ttf_engine>);

        surface render(const std::string_view& text, color color = color::white) const;

        // When sizing text, it's important to know that only the horizontal
        // size remains identical to the resulting render's size. The vertical
        // size is around 10% smaller.
        pixel_size size_text(const std::string_view& text) const;
    };
}