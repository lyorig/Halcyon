#include <halcyon/debug.hpp>
#include <halcyon/other/display.hpp>

using namespace hal;

display::display(index_t disp_idx)
    : m_index { disp_idx }
{
    SDL_DisplayMode dm;

    HAL_ASSERT_VITAL(::SDL_GetDesktopDisplayMode(disp_idx, &dm) == 0, ::SDL_GetError());

    m_size.x = dm.w;
    m_size.y = dm.h;

    m_hz = dm.refresh_rate;
}

display::display(index_t disp_idx, index_t mode_idx)
    : m_index { disp_idx }
{
    SDL_DisplayMode dm;

    HAL_ASSERT_VITAL(::SDL_GetDisplayMode(disp_idx, mode_idx, &dm) == 0, ::SDL_GetError());

    m_size.x = dm.w;
    m_size.y = dm.h;

    m_hz = dm.refresh_rate;
}

display::index_t display::amount()
{
    const auto ret = ::SDL_GetNumVideoDisplays();

    HAL_ASSERT(ret >= 1, ::SDL_GetError());

    return ret;
}

display::index_t display::num_modes() const
{
    const auto ret = ::SDL_GetNumDisplayModes(m_index);

    HAL_ASSERT(ret >= 1, ::SDL_GetError());

    return static_cast<display::index_t>(ret);
}

display::index_t display::idx() const
{
    return m_index;
}

pixel_point display::size() const
{
    return m_size;
}

display::hz_t display::hz() const
{
    return m_hz;
}

std::string_view display::name() const
{
    return name(m_index);
}

std::string_view display::name(index_t idx)
{
    const char* name { ::SDL_GetDisplayName(idx) };

    HAL_ASSERT(name != nullptr, ::SDL_GetError());

    return name;
}