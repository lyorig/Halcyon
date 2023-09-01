#include "font.hpp"

#include "surface.hpp"

using namespace hal;

font::font(const char* path, lyo::u8 size, lyo::pass_key<ttf_engine>) noexcept :
    sdl_object { ::TTF_OpenFont(path, size) }
{
    HAL_DEBUG_PRINT(severity::load, "Loaded font ", path, " with size ", static_cast<lyo::u32>(size));
}

surface font::render(const std::string& text, rgb color) const noexcept
{
    return ::TTF_RenderUTF8_Blended_Wrapped(m_object.get(), text.c_str(), color.to_color(), 0);
}