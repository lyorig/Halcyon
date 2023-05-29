#pragma once

#include <lyo/timer.hpp>

#include "components/renderer.hpp"
#include "input_handler.hpp"
#include "internal/subsystem.hpp"
#include "texture.hpp"
#include "types.hpp"

namespace halcyon
{
    class engine;

    class window : public sdl_object<SDL_Window, ::SDL_DestroyWindow>
    {
      public:

        halcyon::renderer renderer;

        enum flag_t : lyo::u16
        {
            fullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP,
            hidden     = SDL_WINDOW_HIDDEN,
            resizable  = SDL_WINDOW_RESIZABLE,
            minimized  = SDL_WINDOW_MINIMIZED,
            maximized  = SDL_WINDOW_MAXIMIZED
        };

        window(engine& engine, const char* title, const pixel_size& size, lyo::u32 window_flags, lyo::u32 renderer_flags) noexcept;

        void present() noexcept;

        const input_handler& input() const noexcept;
        const pixel_size&    size() const noexcept;
        double               delta_time() const noexcept;

      private:

        input_handler m_input;

        pixel_size m_size;

        lyo::precise_timer m_delta;
    };
}  // namespace halcyon
