#pragma once

#include <SDL2/SDL.h>

#include <halcyon/debug.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>
#include <utility>

// subsystem.hpp:
// A RAII way of initializing SDL subsystems.

namespace hal
{
    class engine;

    enum class system : lyo::u16
    {
        video = SDL_INIT_VIDEO,
        audio = SDL_INIT_AUDIO,
        controller = SDL_INIT_GAMECONTROLLER,
        haptic = SDL_INIT_HAPTIC,
        sensor = SDL_INIT_SENSOR,
        all = SDL_INIT_EVERYTHING
    };

    // A base for SDL subsystem proxies.
    // Meant for internal use. Do not use directly,
    // instead use the derived classes provided below.
    template <system Sys>
    class subsystem
    {
    public:
        enum : std::underlying_type_t<decltype(Sys)>
        {
            system = std::to_underlying(Sys)
        };

        subsystem(const engine& eng)
        {
            HAL_DEBUG_ASSERT_VITAL(::SDL_InitSubSystem(Uint32(Sys)) == 0, ::SDL_GetError());
        }

        ~subsystem()
        {
            ::SDL_QuitSubSystem(Uint32(Sys));
        }
    };

    // Specializations.
    class video : subsystem<system::video>
    {
    public:
        using subsystem::subsystem;

        struct display
        {
            using index = lyo::u8;
            using hz_type = lyo::u16;

            display(const SDL_DisplayMode& mode, lyo::pass_key<video>)
                : size { pixel_type(mode.w), pixel_type(mode.h) }
                , hz { hz_type(mode.refresh_rate) }
            {
            }

            const pixel_size size;
            const hz_type    hz;
        };

        display::index num_displays() const;

        display display(display::index idx) const;

    private:
        SDL_DisplayMode display_mode(display::index idx) const;
    };

    class audio : subsystem<system::audio>
    {
        using subsystem::subsystem;
    };
}