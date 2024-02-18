#pragma once

#include <SDL.h>

#include <halcyon/debug.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>
#include <utility>
#include <vector>

// subsystem.hpp:
// A RAII way of initializing SDL subsystems.

namespace hal
{
    class engine;

    enum class system : lyo::u16
    {
        video      = SDL_INIT_VIDEO,
        audio      = SDL_INIT_AUDIO,
        controller = SDL_INIT_GAMECONTROLLER,
        haptic     = SDL_INIT_HAPTIC,
        sensor     = SDL_INIT_SENSOR,
        all        = SDL_INIT_EVERYTHING
    };

    // A base for SDL subsystem proxies. Meant for internal use.
    // Do not use directly, instead use the derived classes provided below.
    template <system Sys>
    class subsystem
    {
    public:
        enum : std::underlying_type_t<decltype(Sys)>
        {
            system = std::to_underlying(Sys)
        };

        subsystem()
        {
            HAL_ASSERT_VITAL(::SDL_InitSubSystem(Uint32(Sys)) == 0, ::SDL_GetError());
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
        using index_t = lyo::u8;
        using hz_t    = lyo::u16;

        display(index_t disp_idx);

        static index_t amount();

        index_t num_modes() const;

        display mode(index_t mode_idx) const;

        std::vector<display> modes() const;

        index_t     idx() const;
        pixel_point size() const;
        const char* name() const;
        hz_t        hz() const;

    private:
        display(index_t disp_idx, index_t mode_idx);

        const index_t m_index;
    };

    class video : subsystem<system::video>
    {
    public:
        using subsystem::subsystem;
    };
}