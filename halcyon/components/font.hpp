#pragma once

#include <SDL2/SDL_ttf.h>

#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>
#include <lyo/types.hpp>
#include <string>

#include <halcyon/components/sdl_object.hpp>

namespace hal
{
    class ttf_engine;
    class surface;

    class font : public sdl_object<TTF_Font, &::TTF_CloseFont>
    {
    public:
        font(const char* path, lyo::u8 size, lyo::pass_key<ttf_engine>);

        surface render(const std::string& text, color color = color::white) const;

        pixel_size size_text(const char* text) const;
        pixel_size size_text(const std::string& text) const;
    };
} // namespace hal