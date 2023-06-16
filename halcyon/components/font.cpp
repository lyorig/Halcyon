#include "font.hpp"

using namespace hal;

font::font(const char* path, lyo::u8 size) noexcept :
    sdl_object { ::TTF_OpenFont(path, size) }
{
}