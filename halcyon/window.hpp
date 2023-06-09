#pragma once

#include <lyo/timer.hpp>

#include "components/renderer.hpp"
#include "internal/subsystem.hpp"
#include "texture.hpp"

namespace hal
{
    class engine;
    class input_handler;
    struct fullscreen_t  // Tag type for a fullscreen window.
    {
    } constexpr fullscreen;

    class window : public sdl_object<SDL_Window, ::SDL_DestroyWindow>
    {
      public:

        hal::renderer renderer;

        enum flag : lyo::u16
        {
            fullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP,
            hidden     = SDL_WINDOW_HIDDEN,
            resizable  = SDL_WINDOW_RESIZABLE,
            minimized  = SDL_WINDOW_MINIMIZED,
            maximized  = SDL_WINDOW_MAXIMIZED
        };

        window(engine& engine, const char* title, const pixel_size& size, lyo::u32 window_flags, lyo::u32 renderer_flags) noexcept;
        window(engine& engine, const char* title, fullscreen_t, lyo::u32 renderer_flags) noexcept;

        void present() noexcept;

        const pixel_size& size() const noexcept;
        double            delta_time() const noexcept;

      private:

        const pixel_size m_size;

        lyo::precise_timer m_delta;
    };
}  // namespace hal
