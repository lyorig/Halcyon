#include <halcyon/ttf.hpp>

using namespace hal;

ttf::cleanup::cleanup()
{
    HAL_ASSERT_VITAL(::TTF_Init() == 0, ::TTF_GetError());

    HAL_PRINT(severity::init, "Initialized TTF engine");
}

ttf::cleanup::~cleanup() { ::TTF_Quit(); }

bool ttf::cleanup::initialized()
{
    return ::TTF_WasInit() > 0;
}

ttf::font::font(accessor data, lyo::u8 size)
    : object { ::TTF_OpenFontRW(data.get(), false, size) }
{
    HAL_WARN_IF(height() != skip(), '\"', family(), ' ', style(), "\" has different height (", height(), "px) & skip (", skip(), "px). size_text() might not return accurate vertical results.");
}

surface ttf::font::render(const std::string_view& text, color color) const
{
    return ::TTF_RenderUTF8_LCD_Wrapped(this->ptr(), text.data(), color.to_sdl_color(), {}, 0);
}

pixel_point ttf::font::size_text(const std::string_view& text) const
{
    point<int> size;

    ::TTF_SizeUTF8(this->ptr(), text.data(), &size.x, &size.y);

    return pixel_point(size);
}

pixel_t ttf::font::height() const
{
    return static_cast<pixel_t>(::TTF_FontHeight(this->ptr()));
}

pixel_t ttf::font::skip() const
{
    return static_cast<pixel_t>(::TTF_FontLineSkip(this->ptr()));
}

std::string_view ttf::font::family() const
{
    return ::TTF_FontFaceFamilyName(ptr());
}

std::string_view ttf::font::style() const
{
    return ::TTF_FontFaceStyleName(ptr());
}