#pragma once

#include <halcyon/types/other.hpp>
#include <lyo/timer.hpp>

#include "components/renderer.hpp"
#include "internal/subsystem.hpp"
#include "texture.hpp"

namespace hal {
class engine;
class input_handler;

TAG_TYPE(fullscreen_mode);

class window : public sdl_object<SDL_Window, &::SDL_DestroyWindow> {
public:
    using id_type = Uint32;

    enum flags : lyo::u16 {
        none = 0,
        fullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP,
        hidden = SDL_WINDOW_HIDDEN,
        resizeable = SDL_WINDOW_RESIZABLE,
        minimized = SDL_WINDOW_MINIMIZED,
        maximized = SDL_WINDOW_MAXIMIZED
    };

    hal::renderer renderer;

    window(engine& engine, const char* title, const pixel_size& pos, const pixel_size& size, il<flags> w_flags, il<renderer::flags> r_flags) noexcept;
    window(engine& engine, const char* title, fullscreen_mode_tag, il<renderer::flags> r_flags) noexcept;

    void present() const noexcept;

    void set_as_target() const noexcept;

    [[nodiscard]] pixel_size size() const noexcept;

    [[nodiscard]] id_type id() const noexcept;

private:
    pixel_size internal_size() const noexcept;
};
} // namespace hal
