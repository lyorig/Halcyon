#include "subsystem.hpp"

using namespace hal;

video::display::index video::num_displays() const
{
    const auto ret = ::SDL_GetNumVideoDisplays();

    HAL_DEBUG_ASSERT(ret != -1, ::SDL_GetError());

    return ret;
}

struct video::display video::display(display::index idx) const
{
    return { this->display_mode(idx), {} };
}

SDL_DisplayMode video::display_mode(display::index idx) const
{
    HAL_DEBUG_ASSERT(idx < this->num_displays(), "Display index out of range");

    SDL_DisplayMode mode;

    HAL_DEBUG_ASSERT_VITAL(::SDL_GetDesktopDisplayMode(idx, &mode) == 0, ::SDL_GetError());

    return mode;
}