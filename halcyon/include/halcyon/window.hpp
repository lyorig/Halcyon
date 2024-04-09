#pragma once

#include <lyo/timer.hpp>

#include <halcyon/other/display.hpp>
#include <halcyon/texture.hpp>

namespace hal
{
    LYO_TAG(fullscreen);

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

        // Create a window with a specified title, size and flags.
        window(std::string_view title, const pixel_point& size, std::initializer_list<flags> flags);

        // Create a window with a specified title and predefined flags for fullscreen mode.
        window(std::string_view title, fullscreen_tag);

        // Get/set this window's size. Expect weird behavior in fullscreen mode.
        pixel_point size() const;
        void        size(pixel_point sz);

        // Get information about this window's display.
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
