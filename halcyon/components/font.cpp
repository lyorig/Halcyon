#include "font.hpp"

#include "surface.hpp"

using namespace hal;

font::font(const char* path, lyo::u8 size, lyo::pass_key<ttf_engine>)
    : sdl_object { ::TTF_OpenFont(path, size) }
{
    HAL_DEBUG_PRINT(severity::load, "Loaded font ", path, " with size ", static_cast<lyo::u32>(size));
}

surface font::render(const std::string& text, color color) const
{
    return ::TTF_RenderUTF8_Blended_Wrapped(m_object.get(), text.c_str(), color.to_sdl_color(), 0);
}

pixel_size font::size_text(const char* text) const
{
    point<int> size;

    ::TTF_SizeText(this->ptr(), text, &size.x, &size.y);

    return static_cast<pixel_size>(size);
}