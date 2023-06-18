#include "font_loader.hpp"

using namespace hal;

font_loader::font_loader(window& wnd) noexcept
{
    HAL_DEBUG_VERIFY(::TTF_Init() == 0, ::TTF_GetError());
}

font_loader::~font_loader()
{
    ::TTF_Quit();
}

font font_loader::load(const char* path, lyo::u8 size) const noexcept
{
    return { path, size, {} };
}