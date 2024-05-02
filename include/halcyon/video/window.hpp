#pragma once

#include <halcyon/video/display.hpp>
#include <halcyon/video/renderer.hpp>

#include <halcyon/internal/scaler.hpp>

// video/window.hpp:
// A window. Can't do much by itself.

namespace hal
{
    extern template class sdl::object<SDL_Window, ::SDL_DestroyWindow>;

    // A window. Not much more to say.
    class window : public sdl::object<SDL_Window, ::SDL_DestroyWindow>
    {
    public:
        using id_t      = u8;
        using authority = hal::detail::subsystem<hal::detail::system::video>;

        enum class flags : u16
        {
            none                  = 0,
            fullscreen            = SDL_WINDOW_FULLSCREEN,
            fullscreen_borderless = SDL_WINDOW_FULLSCREEN_DESKTOP,
            hidden                = SDL_WINDOW_HIDDEN,
            resizeable            = SDL_WINDOW_RESIZABLE,
            minimized             = SDL_WINDOW_MINIMIZED,
            maximized             = SDL_WINDOW_MAXIMIZED
        };

        window() = default;
        window(SDL_Window* ptr, pass_key<authority> sys);

        renderer make_renderer(std::initializer_list<renderer::flags> flags = { renderer::flags::accelerated });

        pixel_point pos() const;
        void        pos(pixel_point ps);

        // Get/set this window's size. Expect weird behavior in fullscreen mode.
        pixel_point size() const;
        void        size(pixel_point sz);
        void        size(scaler scl);

        // Get the index of the display this window is currently on.
        display::id_t display_index() const;

        // Get/set this window's title.
        std::string_view title() const;
        void             title(std::string_view val);

        id_t id() const;

        // Returns true if the window is fullscreen.
        // Fullscreen borderless mode is also considered as such.
        bool fullscreen() const;
        void fullscreen(bool set);
    };
}