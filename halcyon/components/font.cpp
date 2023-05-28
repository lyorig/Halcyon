#include "font.hpp"



using namespace halcyon;

font::font(const char* path, lyo::u8 size) noexcept :
    sdl_object { ::TTF_OpenFont(path, size), "Couldn't open font" }
{
}