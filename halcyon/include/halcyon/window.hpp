#pragma once

#include <lyo/timer.hpp>

#include <halcyon/other/display.hpp>
#include <halcyon/texture.hpp>

namespace hal
{
    LYO_TAG(fullscreen_mode);

    class window : public sdl::object<SDL_Window, &::SDL_DestroyWindow>
    {
    public:
        enum flags : Uint32
        {
            none                = 0,
            fullscreen          = SDL_WINDOW_FULLSCREEN,
            fullscreen_windowed = SDL_WINDOW_FULLSCREEN_DESKTOP,
            hidden              = SDL_WINDOW_HIDDEN,
            resizeable          = SDL_WINDOW_RESIZABLE,
            minimized           = SDL_WINDOW_MINIMIZED,
            maximized           = SDL_WINDOW_MAXIMIZED
        };

        window(std::string_view title, const pixel_point& size, std::initializer_list<flags> flags);
        window(std::string_view title, fullscreen_mode_tag);

        pixel_point size() const;
        void        set_size(pixel_point sz);

        display display_info() const;

        std::string_view title() const;
    };
} // namespace hal
