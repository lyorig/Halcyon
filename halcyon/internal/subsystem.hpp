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

        subsystem(const engine& eng [[maybe_unused]])
        {
            HAL_DEBUG_ASSERT_VITAL(::SDL_InitSubSystem(Uint32(Sys)) == 0, ::SDL_GetError());
        }

        ~subsystem()
        {
            ::SDL_QuitSubSystem(Uint32(Sys));
        }
    };

    class video;

    class display : SDL_DisplayMode
    {
    public:
        using index = lyo::u8;
        using hz_type = lyo::u16;

        display(index idx, lyo::pass_key<video>);

        index       idx() const;
        pixel_size  size() const;
        const char* name() const;
        hz_type     hz() const;

    private:
        const index m_index;
    };

    class video : subsystem<system::video>
    {
    public:
        using subsystem::subsystem;

        display::index num_displays() const;

        display display_at(display::index idx) const;
    };

    class audio : subsystem<system::audio>
    {
        using subsystem::subsystem;
    };
}