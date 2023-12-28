#pragma once

#include <halcyon/types/other.hpp>
#include <lyo/timer.hpp>

#include <halcyon/components/renderer.hpp>
#include <halcyon/internal/subsystem.hpp>
#include <halcyon/texture.hpp>

namespace hal
{
    class engine;
    class input_handler;

    LYO_TAG_TYPE(fullscreen_mode);

    class window : public sdl_object<SDL_Window, &::SDL_DestroyWindow>
    {
    public:
        using id_type = Uint32;

        enum flags : lyo::u16
        {
            none = 0,
            fullscreen = SDL_WINDOW_FULLSCREEN,
            fullscreen_windowed = SDL_WINDOW_FULLSCREEN_DESKTOP,
            hidden = SDL_WINDOW_HIDDEN,
            resizeable = SDL_WINDOW_RESIZABLE,
            minimized = SDL_WINDOW_MINIMIZED,
            maximized = SDL_WINDOW_MAXIMIZED
        };

        hal::renderer renderer;

        using system = video;

        window(const system& video, const char* title, const pixel_size& pos, const pixel_size& size, il<flags> w_flags, il<renderer::flags> r_flags);
        window(const system& video, const char* title, fullscreen_mode_tag, il<renderer::flags> r_flags);

        void present();

        void set_as_target();

        pixel_size size() const;

        video::display::index display_index() const;

        const char* title() const;

    private:
        pixel_size internal_size() const;
    };
} // namespace hal
