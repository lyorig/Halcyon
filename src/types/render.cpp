#include <halcyon/types/render.hpp>

using namespace hal;

std::string_view hal::to_string(pixel_format fmt)
{
    return ::SDL_GetPixelFormatName(static_cast<Uint32>(fmt));
}