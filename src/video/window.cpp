#include <halcyon/video/window.hpp>

#include <halcyon/event/event.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal;

window::window(SDL_Window* obj, pass_key<authority>)
    : object { obj }
{
    HAL_PRINT(debug::severity::init, "Created window \"", title(), "\", flags = 0x", std::hex, ::SDL_GetWindowFlags(ptr()), ", ID = ", to_printable_int(id()));
}

renderer window::make_renderer(std::initializer_list<renderer::flags> flags)
{
    return { ::SDL_CreateRenderer(ptr(), -1, detail::to_bitmask<std::uint32_t>(flags)), {} };
}

hal::pixel_point window::pos() const
{
    point<int> ret;

    ::SDL_GetWindowPosition(ptr(), &ret.x, &ret.y);

    return ret;
}

void window::pos(hal::pixel_point ps)
{
    ::SDL_SetWindowPosition(ptr(), ps.x, ps.y);
}

hal::pixel_point window::size() const
{
    point<int> size;

    ::SDL_GetWindowSize(this->ptr(), &size.x, &size.y);

    return size;
}

void window::size(pixel_point sz)
{
    HAL_WARN_IF(fullscreen(), "Setting size of fullscreen window - this does nothing");

    ::SDL_SetWindowSize(this->ptr(), sz.x, sz.y);
}

void window::size(scaler scl)
{
    size(scl(size()));
}

display::id_t window::display_index() const
{
    const auto ret = ::SDL_GetWindowDisplayIndex(this->ptr());

    HAL_ASSERT(ret >= 0, debug::last_error());

    return static_cast<display::id_t>(ret);
}

std::string_view window::title() const
{
    return ::SDL_GetWindowTitle(this->ptr());
}

void window::title(std::string_view val)
{
    ::SDL_SetWindowTitle(ptr(), val.data());
}

window::id_t window::id() const
{
    const auto ret = ::SDL_GetWindowID(ptr());

    HAL_ASSERT(ret != 0, debug::last_error());

    return static_cast<window::id_t>(ret);
}

bool window::fullscreen() const
{
    return static_cast<bool>(::SDL_GetWindowFlags(ptr()) & (std::to_underlying(flags::fullscreen) | std::to_underlying(flags::fullscreen_borderless)));
}

void window::fullscreen(bool set)
{
    HAL_ASSERT_VITAL(::SDL_SetWindowFullscreen(
                         ptr(),
                         set ? std::to_underlying(flags::fullscreen_borderless) : 0)
            == 0,
        debug::last_error());
}