#include "font.hpp"

using namespace hal;

font::font(const char* path, lyo::u8 size, lyo::pass_key<font_loader>) noexcept :
    sdl_object { ::TTF_OpenFont(path, size) }
{
}