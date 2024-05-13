#include <halcyon/ttf.hpp>

using namespace hal;

ttf::context::context()
{
    HAL_WARN_IF(initialized(), "TTF context already exists");

    HAL_ASSERT_VITAL(::TTF_Init() == 0, debug::last_error());

    HAL_PRINT(debug::severity::init, "Initialized TTF context");
}

ttf::context::~context()
{
    HAL_ASSERT(initialized(), "TTF context not initialized at destruction");

    ::TTF_Quit();

    HAL_PRINT("Destroyed TTF context");
}

font ttf::context::load(accessor data, u8 pt) &
{
    return { ::TTF_OpenFontRW(data.use(pass_key<context> {}), true, pt), pass_key<context> {} };
}

bool ttf::context::initialized()
{
    return ::TTF_WasInit() > 0;
}

font::font(TTF_Font* ptr, pass_key<ttf::context>)
    : raii_object { ptr }
{
    HAL_WARN_IF(height() != skip(), '\"', family(), ' ', style(), "\" has different height (", height(), "px) & skip (", skip(), "px). size_text() might not return accurate vertical results.");
}

builder::font_text font::render(std::string_view text) const
{
    return { *this, text, pass_key<font> {} };
}

builder::font_glyph font::render(std::uint32_t glyph) const
{
    return { *this, glyph, pass_key<font> {} };
}

pixel_point font::size_text(const std::string_view& text) const
{
    point<int> size;

    ::TTF_SizeUTF8(get(), text.data(), &size.x, &size.y);

    return pixel_point(size);
}

pixel_t font::height() const
{
    return static_cast<pixel_t>(::TTF_FontHeight(get()));
}

pixel_t font::skip() const
{
    return static_cast<pixel_t>(::TTF_FontLineSkip(get()));
}

std::string_view font::family() const
{
    return ::TTF_FontFaceFamilyName(get());
}

std::string_view font::style() const
{
    return ::TTF_FontFaceStyleName(get());
}

using bft = builder::font_text;

bft::font_text(const font& fnt, std::string_view text, pass_key<font> pk)
    : font_builder_base { fnt, pk }
    , m_text { text.data() }
    , m_wrapLength { invalid() }
{
}

bft& bft::wrap(wrap_length_t wl)
{
    m_wrapLength = wl;

    return get_this();
}

surface bft::operator()(font::render_type rt)
{
    using enum font::render_type;

    if (m_wrapLength == invalid()) // Not wrapping.
    {
        switch (rt)
        {
        case solid:
            return { ::TTF_RenderUTF8_Solid(m_font.get(), m_text, static_cast<SDL_Color>(m_fg)), pass_key<font_text> {} };

        case shaded:
            return { ::TTF_RenderUTF8_Shaded(m_font.get(), m_text, static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg)), pass_key<font_text> {} };

        case blended:
            return { ::TTF_RenderUTF8_Blended(m_font.get(), m_text, static_cast<SDL_Color>(m_fg)), pass_key<font_text> {} };

        case lcd:
            return { ::TTF_RenderUTF8_LCD(m_font.get(), m_text, static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg)), pass_key<font_text> {} };
        }
    }

    else
    {
        switch (rt)
        {
        case solid:
            return { ::TTF_RenderUTF8_Solid_Wrapped(m_font.get(), m_text, static_cast<SDL_Color>(m_fg), m_wrapLength), pass_key<font_text> {} };

        case shaded:
            return { ::TTF_RenderUTF8_Shaded_Wrapped(m_font.get(), m_text, static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg), m_wrapLength), pass_key<font_text> {} };

        case blended:
            return { ::TTF_RenderUTF8_Blended_Wrapped(m_font.get(), m_text, static_cast<SDL_Color>(m_fg), m_wrapLength), pass_key<font_text> {} };

        case lcd:
            return { ::TTF_RenderUTF8_LCD_Wrapped(m_font.get(), m_text, static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg), m_wrapLength), pass_key<font_text> {} };
        }
    }

    std::unreachable();
}

using bfg = builder::font_glyph;

bfg::font_glyph(const font& fnt, std::uint32_t glyph, pass_key<font> pk)
    : font_builder_base { fnt, pk }
    , m_glyph { glyph }
{
}

surface bfg::operator()(font::render_type rt)
{
    using enum font::render_type;

    switch (rt)
    {
    case solid:
        return { ::TTF_RenderGlyph32_Solid(m_font.get(), m_glyph, static_cast<SDL_Color>(m_fg)), pass_key<font_glyph> {} };

    case shaded:
        return { ::TTF_RenderGlyph32_Shaded(m_font.get(), m_glyph, static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg)), pass_key<font_glyph> {} };

    case blended:
        return { ::TTF_RenderGlyph32_Blended(m_font.get(), m_glyph, static_cast<SDL_Color>(m_fg)), pass_key<font_glyph> {} };

    case lcd:
        return { ::TTF_RenderGlyph32_LCD(m_font.get(), m_glyph, static_cast<SDL_Color>(m_fg), static_cast<SDL_Color>(m_bg)), pass_key<font_glyph> {} };
    }

    std::unreachable();
}
