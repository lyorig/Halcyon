#include <halcyon/video/display.hpp>

#include <SDL_video.h>

#include <halcyon/debug.hpp>

using namespace hal;

display::display(const sdl::display& src)
    : size { src.size() }
    , hz { src.hz() }
{
}

sdl::display::display(hal::display::id_t id, pass_key<proxy::display>)
{
    HAL_ASSERT_VITAL(::SDL_GetDesktopDisplayMode(id, this) == 0, debug::last_error());
}

sdl::display::display(const hal::display& src)
    : SDL_DisplayMode {
        // TODO: Format???
        .w            = src.size.x,
        .h            = src.size.y,
        .refresh_rate = src.hz,
        .driverdata   = nullptr
    }
{
}

pixel_point sdl::display::size() const
{
    return { w, h };
}

display::hz_t sdl::display::hz() const
{
    return static_cast<hal::display::hz_t>(refresh_rate);
}