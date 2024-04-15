#pragma once

#include <SDL.h>

#include <halcyon/debug.hpp>

namespace hal
{
    class context;

    // Available systems. More will be added as Halcyon adds
    // more features from SDL.
    enum class system
    {
        video  = SDL_INIT_VIDEO,
        events = SDL_INIT_EVENTS
    };

    constexpr std::string_view to_string(system s)
    {
        using enum system;

        switch (s)
        {
        case video:
            return "Video";

        case events:
            return "Events";
        }
    }

    namespace detail
    {
        // A subsystem that doesn't actually (de)initialize anything.
        // This is more of a base class to ensure dependent classes
        // accept proxies as well as actual self-initialized systems.
        template <system S>
        class subsystem
        {
        public:
            subsystem([[maybe_unused]] context& ctx)
            {
            }

            subsystem(const subsystem&) = delete;

            static bool initialized()
            {
                return ::SDL_WasInit(static_cast<std::uint32_t>(S)) == static_cast<std::uint32_t>(S);
            }
        };

        // A subsystem that initializes itself.
        template <system S>
        class subinit : public subsystem<S>
        {
        public:
            subinit([[maybe_unused]] context& ctx)
                : subsystem<S> { ctx }
            {
                HAL_WARN_IF(initialized(), "Subsystem \"", to_string(S), "\" is already initialized");
                HAL_ASSERT_VITAL(::SDL_InitSubSystem(static_cast<std::uint32_t>(S)) == 0, debug::last_error());
            }

            subinit(const subinit&) = delete;

            ~subinit()
            {
                ::SDL_QuitSubSystem(static_cast<std::uint32_t>(S));
            }

            static bool initialized()
            {
                return ::SDL_WasInit(static_cast<std::uint32_t>(S)) == static_cast<std::uint32_t>(S);
            }
        };
    }
}