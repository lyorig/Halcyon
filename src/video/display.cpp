#include <halcyon/video/display.hpp>

#include <SDL_video.h>

#include <halcyon/debug.hpp>

using namespace hal;

info::display::display(const sdl::display& src)
    : size { src.size() }
    , fmt { src.format() }
    , hz { src.hz() }
{
}

info::sdl::display::display(hal::display::id_t id, pass_key<proxy::display>)
{
    HAL_ASSERT_VITAL(::SDL_GetDesktopDisplayMode(id, this) == 0, debug::last_error());
}

info::sdl::display::display(const info::display& src)
    : SDL_DisplayMode {
        .format       = static_cast<Uint32>(src.fmt),
        .w            = src.size.x,
        .h            = src.size.y,
        .refresh_rate = src.hz,
        .driverdata   = nullptr
    }
{
}

pixel_point info::sdl::display::size() const
{
    return { w, h };
}

pixel_format info::sdl::display::format() const
{
    return static_cast<pixel_format>(SDL_DisplayMode::format);
}

hal::display::hz_t info::sdl::display::hz() const
{
    return static_cast<hal::display::hz_t>(refresh_rate);
}

std::ostream& hal::info::sdl::operator<<(std::ostream& str, const info::display& disp)
{
    return str << "[size: " << disp.size << ", format: " << to_string(disp.fmt) << ", Hz: " << disp.hz << ']';
}