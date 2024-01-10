#include <halcyon/components/font.hpp>

#include <halcyon/components/surface.hpp>

using namespace hal;

font::font(const char* path, lyo::u8 size, lyo::pass_key<ttf_engine>)
    : object { ::TTF_OpenFont(path, size) }
{
    HAL_DEBUG_PRINT(debug::load, "Loaded font ", path, " with size ", lyo::u32(size));
}

surface font::render(const std::string_view& text, color color) const
{
    HAL_DEBUG_ASSERT(!text.contains('\n'), "Newline detected; use the wrapped overload");

    surface temp { ::TTF_RenderUTF8_Solid(this->ptr(), text.data(), color.to_sdl_color()) };
    HAL_DEBUG_ASSERT(temp.exists(), ::TTF_GetError());

    // Some texture have more space than necessary - get rid of that shiiiiit.
    return temp.clip({ 0, 0, temp.size().x, this->height() });
}

surface font::render(wrapped_tag, const std::string_view& text, color color) const
{
    return ::TTF_RenderUTF8_Solid_Wrapped(this->ptr(), text.data(), color.to_sdl_color(), 0);
}

pixel_point font::size_text(const std::string_view& text) const
{
    point<int> size;

    ::TTF_SizeUTF8(this->ptr(), text.data(), &size.x, &size.y);

    return pixel_point(size);
}

pixel_t font::height() const
{
    return ::TTF_FontHeight(this->ptr());
}

pixel_t font::skip() const
{
    return ::TTF_FontLineSkip(this->ptr());
}