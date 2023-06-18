#include "surface.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "font.hpp"

using namespace hal;

surface::surface(const char* path, lyo::pass_key<image_loader>) noexcept :
    sdl_object { ::IMG_Load(path) }
{
}

surface::surface(const font& fnt, const char* text, color clr) noexcept :
    sdl_object { ::TTF_RenderUTF8_Blended_Wrapped(fnt.ptr(), text, convert_color(clr), 0) }
{
}

pixel_size surface::size() const noexcept
{
    return { static_cast<pixel_t>(m_object->w), static_cast<pixel_t>(m_object->h) };
}

constexpr SDL_Color surface::convert_color(color clr) noexcept
{
    return SDL_Color {
        static_cast<Uint8>(clr >> 16 & 0xFF),
        static_cast<Uint8>(clr >> 8 & 0xFF),
        static_cast<Uint8>(clr & 0xFF),
        255
    };
}