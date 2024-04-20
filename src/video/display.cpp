#include <halcyon/video/display.hpp>

#include <SDL3/SDL_video.h>

#include <halcyon/debug.hpp>

using namespace hal::video;

display::display(id_t disp_idx, pass_key<system>)
    : m_ptr { ::SDL_GetDesktopDisplayMode(disp_idx) }
{
    HAL_ASSERT_VITAL(m_ptr != nullptr, debug::last_error());
}

hal::pixel_point display::size() const
{
    return { static_cast<hal::pixel_t>(m_ptr->w), static_cast<hal::pixel_t>(m_ptr->h) };
}

display::hz_t display::hz() const
{
    return static_cast<display::hz_t>(m_ptr->refresh_rate);
}

display::id_t display::index() const
{
    return static_cast<display::id_t>(m_ptr->displayID);
}

std::string_view display::name() const
{
    const char* name { ::SDL_GetDisplayName(index()) };

    HAL_ASSERT(name != nullptr, debug::last_error());

    return name;
}