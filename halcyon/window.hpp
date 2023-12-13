#pragma once

#include <halcyon/types/other.hpp>
#include <lyo/timer.hpp>

#include "components/renderer.hpp"
#include "internal/subsystem.hpp"
#include "texture.hpp"

namespace hal
{
    class engine;
    class input_handler;

    TAG_TYPE(fullscreen_mode);

    class window : public sdl_object<SDL_Window, &::SDL_DestroyWindow>
    {
    public:
        using id_type = Uint32;

        enum flags : lyo::u16
        {
            none = 0,
            fullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP,
            hidden = SDL_WINDOW_HIDDEN,
            resizeable = SDL_WINDOW_RESIZABLE,
            minimized = SDL_WINDOW_MINIMIZED,
            maximized = SDL_WINDOW_MAXIMIZED
        };

        hal::renderer renderer;

        window(engine& engine, const char* title, const pixel_size& pos, const pixel_size& size, il<flags> w_flags, il<renderer::flags> r_flags);
        window(engine& engine, const char* title, fullscreen_mode_tag, il<renderer::flags> r_flags);

        void present() const;

        void set_as_target() const;

        [[nodiscard]] pixel_size size() const;

        [[nodiscard]] id_type id() const;

    private:
        pixel_size internal_size() const;
    };
} // namespace hal
