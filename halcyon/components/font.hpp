#pragma once

#include <SDL_ttf.h>

#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>
#include <string_view>

#include <halcyon/components/sdl_object.hpp>

namespace hal
{
    class ttf_engine;
    class surface;

    class font : public sdl::object<TTF_Font, &::TTF_CloseFont>
    {
    public:
        font(const char* path, lyo::u8 size, lyo::pass_key<ttf_engine>);

        surface render(const std::string_view& text, color color = color::white) const;

        // When sizing text, it's important to know that the vertical size
        // doesn't necessarily have to match that of the rendered surface.
        pixel_point size_text(const std::string_view& text) const;

        pixel_t height() const;
        pixel_t skip() const;
    };
}