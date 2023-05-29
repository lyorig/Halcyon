#pragma once

#include <SDL2/SDL.h>

#include <lyo/types.hpp>

/* subsystem.hpp:
   A simple way to initialize SDL subsystems. */

namespace halcyon
{
    enum subsystem_t : lyo::u16
    {
        video      = SDL_INIT_VIDEO,
        audio      = SDL_INIT_AUDIO,
        controller = SDL_INIT_GAMECONTROLLER,
        haptic     = SDL_INIT_HAPTIC,
        sensor     = SDL_INIT_SENSOR,
        all        = SDL_INIT_EVERYTHING
    };

    template <subsystem_t System>
    class subsystem
    {
      public:

        subsystem() noexcept
        {
            ::SDL_InitSubSystem(static_cast<Uint32>(System));
        }

        ~subsystem()
        {
            ::SDL_QuitSubSystem(static_cast<Uint32>(System));
        }
    };
}  // namespace halcyon