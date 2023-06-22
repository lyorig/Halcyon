#include "font.hpp"

using namespace hal;

font::font(const char* path, lyo::u8 size, lyo::pass_key<font_loader>) noexcept :
    sdl_object { ::TTF_OpenFont(path, size) }
{
}

surface font::textify(const char* text, color clr) const noexcept
{
    SDL_Surface* surf { ::TTF_RenderUTF8_Blended_Wrapped(m_object, text, sdl_color_from(static_cast<color_type>(clr)), 0) };

    HAL_CHECK(surf != nullptr, ::TTF_GetError());

    return surf;
}

constexpr SDL_Color font::sdl_color_from(color_type clr) noexcept
{
    return SDL_Color {
        static_cast<Uint8>(clr >> 16 & 0xFF),
        static_cast<Uint8>(clr >> 8 & 0xFF),
        static_cast<Uint8>(clr & 0xFF),
        255
    };
}