#include <halcyon/internal/subsystem.hpp>

using namespace hal;

display::display(display::index idx, lyo::pass_key<video>)
    : m_index { idx }
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_GetDesktopDisplayMode(idx, static_cast<SDL_DisplayMode*>(this)) == 0, ::SDL_GetError());
}

display::index display::idx() const
{
    return m_index;
}

pixel_size display::size() const
{
    return { pixel_type(w), pixel_type(h) };
}

display::hz_type display::hz() const
{
    return static_cast<display::hz_type>(refresh_rate);
}

const char* display::name() const
{
    const char* name { ::SDL_GetDisplayName(this->idx()) };

    HAL_DEBUG_ASSERT(name != nullptr, ::SDL_GetError());

    return name;
}

display::index video::num_displays() const
{
    const auto ret = ::SDL_GetNumVideoDisplays();

    HAL_DEBUG_ASSERT(ret != -1, ::SDL_GetError());

    return static_cast<display::index>(ret);
}

display video::display_at(display::index idx) const
{
    return { idx, {} };
}