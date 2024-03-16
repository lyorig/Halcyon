#include <halcyon/ttf.hpp>

using namespace hal;

ttf_engine::ttf_engine()
{
    HAL_ASSERT_VITAL(::TTF_Init() == 0, ::TTF_GetError());

    HAL_PRINT(debug::init, "Initialized TTF engine");
}

ttf_engine::~ttf_engine() { ::TTF_Quit(); }

bool ttf_engine::initialized()
{
    return ::TTF_WasInit() > 0;
}

font::font(std::string_view path, lyo::u8 size)
    : object { ::TTF_OpenFont(path.data(), size) }
{
}

surface font::render(const std::string_view& text, color color) const
{
    return { ::TTF_RenderUTF8_LCD_Wrapped(this->ptr(), text.data(), color.to_sdl_color(), {}, 0), lyo::pass_key<font> {} };
}

pixel_point font::size_text(const std::string_view& text) const
{
    point<int> size;

    ::TTF_SizeUTF8(this->ptr(), text.data(), &size.x, &size.y);

    return pixel_point(size);
}

pixel_t font::height() const
{
    return static_cast<pixel_t>(::TTF_FontHeight(this->ptr()));
}

pixel_t font::skip() const
{
    return static_cast<pixel_t>(::TTF_FontLineSkip(this->ptr()));
}