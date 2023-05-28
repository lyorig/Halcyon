#include "ttf_engine.hpp"

using namespace halcyon;

ttf_engine::ttf_engine(engine& engine) noexcept
{
    HALCYON_VERIFY(::TTF_Init() == 0, "SDL_ttf initialization failed");
}

ttf_engine::~ttf_engine()
{
    ::TTF_Quit();
}

font ttf_engine::load_font(const char* path, lyo::u8 size) const noexcept
{
    return { path, size };
}

surface ttf_engine::load_text(const font& font, const char* text) const noexcept
{
    return { font, text };
}

surface ttf_engine::load_text(const font& font, const char* text, color_t color) const noexcept
{
    return { font, text, color };
}