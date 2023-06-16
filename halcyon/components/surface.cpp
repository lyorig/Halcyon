#include "surface.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "font.hpp"

using namespace hal;

surface::surface(const char* path) noexcept :
    sdl_object { ::IMG_Load(path) }
{
}

surface::surface(const font& fnt, const char* text) noexcept :
    sdl_object { ::TTF_RenderUTF8_Blended_Wrapped(fnt.ptr(), text, { 255, 255, 255, 255 }, 0) }
{
}

surface::surface(const font& fnt, const char* text, color_t color) noexcept :
    sdl_object { ::TTF_RenderUTF8_Blended_Wrapped(fnt.ptr(), text, { static_cast<Uint8>(color >> 16), static_cast<Uint8>(color >> 8), static_cast<Uint8>(color), 255 }, 0) }
{
}

pixel_size surface::size() const noexcept
{
    return { static_cast<pixel_t>(m_object->w), static_cast<pixel_t>(m_object->h) };
}