#pragma once

#include <lyoSTL/timer.hpp>

#include <halcyon/internal/subsystem.hpp>
#include <halcyon/texture.hpp>

namespace hal
{
    LYOSTL_TAG(fullscreen_mode);

    class window : public SDL::object<SDL_Window, &::SDL_DestroyWindow>
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

        window(video& sys, const char* title, const pixel_size& pos, const pixel_size& size, std::initializer_list<flags> flags);
        window(video& sys, const char* title, fullscreen_mode_tag);

        pixel_size size() const;

        display::index display_index() const;

        const char* title() const;
    };
} // namespace hal
