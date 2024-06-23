#include <halcyon/video/window.hpp>

#include <halcyon/video.hpp>

using namespace hal;

// Const window view.

using cv = view<const window>;

cv::view(view<const renderer> rnd, pass_key<view<const renderer>>)
    : view_base { ::SDL_RenderGetWindow(rnd.get()) }
{
}

hal::pixel::point cv::pos() const
{
    point<int> ret;

    ::SDL_GetWindowPosition(get(), &ret.x, &ret.y);

    return ret;
}

hal::pixel::point cv::size() const
{
    point<int> size;

    ::SDL_GetWindowSize(get(), &size.x, &size.y);

    return size;
}

display::id_t cv::display_index() const
{
    const auto ret = ::SDL_GetWindowDisplayIndex(get());

    HAL_ASSERT(ret >= 0, debug::last_error());

    return static_cast<display::id_t>(ret);
}

pixel::format cv::pixel_format() const
{
    return static_cast<pixel::format>(::SDL_GetWindowPixelFormat(get()));
}

std::string_view cv::title() const
{
    return ::SDL_GetWindowTitle(get());
}

window::id_t cv::id() const
{
    const auto ret = ::SDL_GetWindowID(get());

    HAL_ASSERT(ret != 0, debug::last_error());

    return static_cast<window::id_t>(ret);
}

bool cv::fullscreen() const
{
    return static_cast<bool>(::SDL_GetWindowFlags(get()) & (std::to_underlying(window::flags::fullscreen) | std::to_underlying(window::flags::fullscreen_borderless)));
}

view<const surface> cv::surface() const
{
    return { ::SDL_GetWindowSurface(get()), pass_key<cv> {} };
}

// Window view.

using v = view<window>;

v::view(view<renderer> rnd, pass_key<view<renderer>>)
    : view { ::SDL_RenderGetWindow(rnd.get()) }
{
}

renderer v::make_renderer(std::initializer_list<renderer::flags> flags) &
{
    return { *this, flags };
}

void v::pos(hal::pixel::point ps)
{
    ::SDL_SetWindowPosition(get(), ps.x, ps.y);
}

void v::size(pixel::point sz)
{
    HAL_WARN_IF(fullscreen(), "Setting size of fullscreen window - this does nothing");

    ::SDL_SetWindowSize(get(), sz.x, sz.y);
}

void v::size(scaler scl)
{
    size(scl(size()));
}

void v::title(std::string_view val)
{
    ::SDL_SetWindowTitle(get(), val.data());
}

void v::fullscreen(bool set)
{
    HAL_ASSERT_VITAL(::SDL_SetWindowFullscreen(
                         get(),
                         set * std::to_underlying(window::flags::fullscreen))
            == 0,
        debug::last_error());
}

window::window(proxy::video&, std::string_view title, pixel::point size, std::initializer_list<flags> flags)
    : raii_object { ::SDL_CreateWindow(title.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.x, size.y, detail::to_bitmask<Uint32>(flags)) }
{
    HAL_PRINT(debug::severity::init, "Created window \"", title, "\" [ID: ", to_printable_int(id()), ']');
}

window::window(proxy::video& sys, std::string_view title, HAL_TAG_NAME(fullscreen))
    : window { sys, title, sys.displays[0].size(), { flags::fullscreen } }
{
}