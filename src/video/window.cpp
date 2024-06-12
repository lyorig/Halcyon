#include <halcyon/video/window.hpp>

#include <halcyon/internal/helpers.hpp>

#include <halcyon/video.hpp>

using namespace hal;

window::window(proxy::video&, std::string_view title, pixel::point size, std::initializer_list<flags> flags)
    : raii_object { ::SDL_CreateWindow(title.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.x, size.y, detail::to_bitmask<Uint32>(flags)) }
{
    HAL_PRINT(debug::severity::init, "Created window \"", title, "\" [ID: ", to_printable_int(id()), ']');
}

window::window(proxy::video& sys, std::string_view title, HAL_TAG_NAME(fullscreen))
    : window { sys, title, sys.displays[0].size(), { flags::fullscreen } }
{
}

renderer window::make_renderer(std::initializer_list<renderer::flags> flags) &
{
    return { *this, flags };
}

hal::pixel::point window::pos() const
{
    point<int> ret;

    ::SDL_GetWindowPosition(get(), &ret.x, &ret.y);

    return ret;
}

void window::pos(hal::pixel::point ps)
{
    ::SDL_SetWindowPosition(get(), ps.x, ps.y);
}

hal::pixel::point window::size() const
{
    point<int> size;

    ::SDL_GetWindowSize(get(), &size.x, &size.y);

    return size;
}

void window::size(pixel::point sz)
{
    HAL_WARN_IF(fullscreen(), "Setting size of fullscreen window - this does nothing");

    ::SDL_SetWindowSize(get(), sz.x, sz.y);
}

void window::size(scaler scl)
{
    size(scl(size()));
}

display::id_t window::display_index() const
{
    const auto ret = ::SDL_GetWindowDisplayIndex(get());

    HAL_ASSERT(ret >= 0, debug::last_error());

    return static_cast<display::id_t>(ret);
}

pixel::format window::pixel_format() const
{
    return static_cast<pixel::format>(::SDL_GetWindowPixelFormat(get()));
}

std::string_view window::title() const
{
    return ::SDL_GetWindowTitle(get());
}

void window::title(std::string_view val)
{
    ::SDL_SetWindowTitle(get(), val.data());
}

window::id_t window::id() const
{
    const auto ret = ::SDL_GetWindowID(get());

    HAL_ASSERT(ret != 0, debug::last_error());

    return static_cast<window::id_t>(ret);
}

bool window::fullscreen() const
{
    return static_cast<bool>(::SDL_GetWindowFlags(get()) & (std::to_underlying(flags::fullscreen) | std::to_underlying(flags::fullscreen_borderless)));
}

void window::fullscreen(bool set)
{
    HAL_ASSERT_VITAL(::SDL_SetWindowFullscreen(
                         get(),
                         set * std::to_underlying(flags::fullscreen))
            == 0,
        debug::last_error());
}

const surface* window::surface() const
{
    const SDL_Surface* ptr { ::SDL_GetWindowSurface(get()) };

    HAL_ASSERT(ptr != nullptr, debug::last_error());

    return reinterpret_cast<const class surface*>(ptr);
}