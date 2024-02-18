#include <halcyon/other/subsystem.hpp>
#include <vector>

using namespace hal;

display::display(display::index_t disp_idx, lyo::pass_key<video>)
    : m_index { disp_idx }
{
    HAL_ASSERT_VITAL(::SDL_GetDesktopDisplayMode(disp_idx, static_cast<SDL_DisplayMode*>(this)) == 0, ::SDL_GetError());
}

display::display(display::index_t disp_idx, display::index_t mode_idx)
    : m_index { disp_idx }
{
    HAL_ASSERT_VITAL(::SDL_GetDisplayMode(disp_idx, mode_idx, static_cast<SDL_DisplayMode*>(this)) == 0, ::SDL_GetError());
}

display::index_t display::num_modes() const
{
    const auto ret = ::SDL_GetNumDisplayModes(m_index);

    HAL_ASSERT(ret >= 1, ::SDL_GetError());

    return static_cast<display::index_t>(ret);
}

display display::mode_at(display::index_t mode_idx) const
{
    return { m_index, mode_idx };
}

std::vector<display> display::modes() const
{
    std::vector<display> ret;
    const index_t        mode_amount { this->num_modes() };

    ret.reserve(mode_amount);

    for (index_t i { 0 }; i < mode_amount; ++i)
        ret.push_back({ m_index, i });

    return ret;
}

display::index_t display::idx() const
{
    return m_index;
}

pixel_point display::size() const
{
    return { pixel_t(w), pixel_t(h) };
}

display::hz_type_t display::hz() const
{
    return static_cast<display::hz_type_t>(refresh_rate);
}

const char* display::name() const
{
    const char* name { ::SDL_GetDisplayName(this->idx()) };

    HAL_ASSERT(name != nullptr, ::SDL_GetError());

    return name;
}

display::index_t video::num_displays() const
{
    const auto ret = ::SDL_GetNumVideoDisplays();

    HAL_ASSERT(ret != -1, ::SDL_GetError());

    return static_cast<display::index_t>(ret);
}

display video::display_at(display::index_t idx) const
{
    return { idx, lyo::pass_key<video> {} };
}