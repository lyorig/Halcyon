#pragma once

#include <SDL2/SDL.h>

#include <halcyon/debug.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>
#include <utility>

// subsystem.hpp:
// A RAII way of initializing SDL subsystems.

#define HAL_SUBSYSTEM_BASE(sys)                  \
    enum : std::underlying_type_t<decltype(sys)> \
    {                                            \
        system = std::to_underlying(sys)         \
    };                                           \
    subsystem()                                  \
    {                                            \
        ::SDL_InitSubSystem(Uint32(sys));        \
    }                                            \
    ~subsystem()                                 \
    {                                            \
        ::SDL_QuitSubSystem(Uint32(sys));        \
    }

namespace hal
{
    enum class subsys : lyo::u16
    {
        video = SDL_INIT_VIDEO,
        audio = SDL_INIT_AUDIO,
        controller = SDL_INIT_GAMECONTROLLER,
        haptic = SDL_INIT_HAPTIC,
        sensor = SDL_INIT_SENSOR,
        all = SDL_INIT_EVERYTHING
    };

    // Primary template.
    template <subsys System>
    class subsystem
    {
    public:
        HAL_SUBSYSTEM_BASE(System);
    };

    struct display_info
    {
        using index = lyo::u8;
        using hz_type = lyo::u16;

        display_info(const SDL_DisplayMode& mode, lyo::pass_key<subsystem<subsys::video>>)
            : size { pixel_type(mode.w), pixel_type(mode.h) }
            , hz { hz_type(mode.refresh_rate) }
        {
        }

        pixel_size size;
        hz_type    hz;
    };

    // Specializations.
    template <>
    class subsystem<subsys::video>
    {
    public:
        HAL_SUBSYSTEM_BASE(subsys::video);

        display_info::index num_displays() const
        {
            const auto ret = ::SDL_GetNumVideoDisplays();

            HAL_DEBUG_ASSERT(ret != -1, ::SDL_GetError());

            return ret;
        }

        display_info display(display_info::index idx) const
        {
            return { this->display_mode(idx), {} };
        }

    private:
        SDL_DisplayMode display_mode(display_info::index idx) const
        {
            HAL_DEBUG_ASSERT(idx < this->num_displays(), "Display index out of range");

            SDL_DisplayMode mode;

            HAL_DEBUG_ASSERT_VITAL(::SDL_GetDesktopDisplayMode(idx, &mode) == 0, ::SDL_GetError());

            return mode;
        }
    };
} // namespace hal