#include "display_info.hpp"

using namespace hal;

display_info::display_info(const SDL_DisplayMode& dm) noexcept :
    size { static_cast<pixel_t>(dm.w), static_cast<pixel_t>(dm.h) },
    hz { static_cast<lyo::u16>(dm.refresh_rate) },
    bpp { SDL_BITSPERPIXEL(dm.format) }
{
}