#include "font.hpp"

#include <halcyon/internal/config.hpp>

#include "surface.hpp"

using namespace hal;

font::font(const char* path, lyo::u8 size, lyo::pass_key<font_loader>) noexcept :
    sdl_object { ::TTF_OpenFont(path, size) }
{
    HAL_DEBUG_PRINT(severity::load, "Loaded font ", path, " with size ", static_cast<lyo::u32>(size));
}

surface font::render(const std::string& text, color clr) const noexcept
{
    return ::TTF_RenderUTF8_Blended_Wrapped(m_object.get(), text.c_str(), sdl_color_from(static_cast<color_type>(clr)), 0);
}

constexpr SDL_Color font::sdl_color_from(color_type clr) noexcept
{
    if constexpr (!cfg::performance_mode)
    {
        return SDL_Color {
            static_cast<Uint8>(clr >> 16 & 0xFF),
            static_cast<Uint8>(clr >> 8 & 0xFF),
            static_cast<Uint8>(clr & 0xFF),
            255
        };
    }
    else
    {
        return SDL_Color {
            static_cast<Uint8>(clr >> 16),
            static_cast<Uint8>(clr >> 8),
            static_cast<Uint8>(clr),
            255
        };
    }
}