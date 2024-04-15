#pragma once

#include <halcyon/internal/sdl_object.hpp>

#include <halcyon/video/display.hpp>

#include <halcyon/internal/subsystem.hpp>

// video/window.hpp:
// A window. Can't do much by itself.

namespace hal
{
    // A window. Not much more to say.
    class window : public sdl::object<SDL_Window, &::SDL_DestroyWindow>
    {
    public:
        using id_t = lyo::u8;

        enum class flags : lyo::u16
        {
            none                  = 0,
            fullscreen            = SDL_WINDOW_FULLSCREEN,
            fullscreen_borderless = SDL_WINDOW_FULLSCREEN_DESKTOP,
            hidden                = SDL_WINDOW_HIDDEN,
            resizeable            = SDL_WINDOW_RESIZABLE,
            minimized             = SDL_WINDOW_MINIMIZED,
            maximized             = SDL_WINDOW_MAXIMIZED
        };

        window(detail::subsystem<system::video>&, std::string_view name, pixel_point size, std::initializer_list<flags> flags = {});

        // Get/set this window's size. Expect weird behavior in fullscreen mode.
        pixel_point size() const;
        void        size(pixel_point sz);

        // Get the index of the display this window is currently on.
        display::index_t display_index() const;

        // Get/set this window's title.
        std::string_view title() const;
        void             title(std::string_view val);

        id_t id() const;

        // Returns true if the window is fullscreen.
        // Fullscreen borderless mode is also considered as such.
        bool fullscreen() const;
    };
} // namespace hal
