#include <halcyon/video/window.hpp>

#include <halcyon/event/handler.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal::video;

window::window(authority&, std::string_view name, pixel_point size, std::initializer_list<flags> flags)
    : object { ::SDL_CreateWindow(name.data(), size.x, size.y, detail::to_bitmask<std::uint32_t>(flags)) }
{
    HAL_PRINT(debug::severity::init, "Created window \"", title(), "\", flags = 0x", std::hex, detail::to_bitmask<std::uint32_t>(flags), ", ID = ", to_printable_int(id()));
}

void window::sync()
{
    const auto ret = ::SDL_SyncWindow(ptr());

    HAL_ASSERT(ret >= 0, debug::last_error());
    HAL_WARN_IF(ret > 0, "Window sync timed out before queued operations completed");
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

display::id_t window::display_index() const
{
    const auto ret = ::SDL_GetDisplayForWindow(this->ptr());

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
    return static_cast<bool>(::SDL_GetWindowFlags(ptr()) & std::to_underlying(flags::fullscreen));
}

void window::fullscreen(bool set)
{
    HAL_ASSERT_VITAL(::SDL_SetWindowFullscreen(
                         ptr(),
                         static_cast<SDL_bool>(set))
            == 0,
        debug::last_error());
}