#include <halcyon/ttf.hpp>

using namespace hal::ttf;

context::context()
{
    HAL_WARN_IF(initialized(), "TTF context already exists");

    HAL_ASSERT_VITAL(::TTF_Init() == 0, debug::last_error());

    HAL_PRINT(debug::severity::init, "Initialized TTF context");
}

context::~context()
{
    HAL_ASSERT(initialized(), "TTF context not initialized at destruction");

    ::TTF_Quit();

    HAL_PRINT("Destroyed TTF context");
}

font context::load(accessor data, u8 pt) &
{
    return { *this, std::move(data), pt };
}

bool context::initialized()
{
    return ::TTF_WasInit() > 0;
}

font::font(context& auth, accessor data, u8 pt)
    : object { ::TTF_OpenFontRW(data.get(pass_key<ttf::font> {}), true, pt) }
{
    HAL_WARN_IF(height() != skip(), '\"', family(), ' ', style(), "\" has different height (", height(), "px) & skip (", skip(), "px). size_text() might not return accurate vertical results.");
}

font::~font()
{
    HAL_ASSERT(ttf::context::initialized(), "TTF context inactive in font destructor");
}

hal::surface font::render(std::string_view text, hal::color color) const
{
    return { *this, text, color };
}

hal::pixel_point font::size_text(const std::string_view& text) const
{
    point<int> size;

    ::TTF_SizeUTF8(this->ptr(), text.data(), &size.x, &size.y);

    return pixel_point(size);
}

hal::pixel_t font::height() const
{
    return static_cast<pixel_t>(::TTF_FontHeight(this->ptr()));
}

hal::pixel_t font::skip() const
{
    return static_cast<pixel_t>(::TTF_FontLineSkip(this->ptr()));
}

std::string_view font::family() const
{
    return ::TTF_FontFaceFamilyName(ptr());
}

std::string_view font::style() const
{
    return ::TTF_FontFaceStyleName(ptr());
}