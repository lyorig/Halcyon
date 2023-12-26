#include "font.hpp"

#include <halcyon/components/surface.hpp>

using namespace hal;

font::font(const char* path, lyo::u8 size, lyo::pass_key<ttf_engine>)
    : sdl_object { ::TTF_OpenFont(path, size) }
{
    HAL_DEBUG_PRINT(debug::load, "Loaded font ", path, " with size ", lyo::u32(size));
}

surface font::render(const std::string& text, color color) const
{
    return ::TTF_RenderUTF8_Blended_Wrapped(this->ptr(), text.c_str(), color.to_sdl_color(), 0);
}

pixel_size font::size_text(const char* text) const
{
    point<int> size;

    ::TTF_SizeText(this->ptr(), text, &size.x, &size.y);

    return pixel_size(size);
}

pixel_size font::size_text(const std::string& text) const
{
    return this->size_text(text.c_str());
}