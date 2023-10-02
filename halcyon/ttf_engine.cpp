#include "ttf_engine.hpp"

using namespace hal;

ttf_engine::ttf_engine(window& wnd) noexcept
{
    HAL_DEBUG_ASSERT(::TTF_Init() == 0, ::TTF_GetError());

    HAL_DEBUG_PRINT(severity::init, "Initialized TTF Engine");
}

ttf_engine::~ttf_engine()
{
    ::TTF_Quit();
}

font ttf_engine::load_font(const char* path, lyo::u8 size) & noexcept
{
    return { path, size, {} };
}

font ttf_engine::load_font(rvalue_font_tag, const char* path, lyo::u8 size) && noexcept
{
    return { path, size, {} };
}