#pragma once

#include <halcyon/video/display.hpp>
#include <halcyon/video/renderer.hpp>

#include <halcyon/internal/scaler.hpp>

#include <halcyon/internal/subsystem.hpp>
#include <halcyon/surface.hpp>

// video/window.hpp:
// A window. Can't do much by itself.

namespace hal
{
    namespace detail
    {
        template <>
        class view_impl<SDL_Window> : public view_base<SDL_Window>
        {
        public:
            using view_base::view_base;

            view_impl(SDL_Window* ptr, pass_key<view::renderer>);

            pixel::point pos() const;

            pixel::point size() const;

            // Get the index of the display this window is currently on.
            display::id_t display_index() const;

            pixel::format pixel_format() const;

            std::string_view title() const;

            u8 id() const;

            // Returns true if the window is fullscreen or fullscreen borderless.
            bool fullscreen() const;

            // View the surface associated with this window.
            view::surface surface() const;
        };
    }

    HAL_TAG(fullscreen);

    // A window. Not much more to say.
    class window : public detail::raii_object<SDL_Window, ::SDL_DestroyWindow>
    {
    public:
        using id_t = u8;

        using authority_t = hal::proxy::video;

        enum class flags : u16
        {
            none                  = 0,
            fullscreen            = SDL_WINDOW_FULLSCREEN,
            fullscreen_borderless = SDL_WINDOW_FULLSCREEN_DESKTOP,
            hidden                = SDL_WINDOW_HIDDEN,
            resizable             = SDL_WINDOW_RESIZABLE,
            minimized             = SDL_WINDOW_MINIMIZED,
            maximized             = SDL_WINDOW_MAXIMIZED
        };

        window() = default;

        window(proxy::video& sys, std::string_view title, pixel::point size, std::initializer_list<flags> flags = {});

        // Create a window in fullscreen window.
        // Warning: This has some issues on macOS due to its DPI scaling stuff.
        window(proxy::video& sys, std::string_view title, HAL_TAG_NAME(fullscreen));

        [[nodiscard]] renderer make_renderer(std::initializer_list<renderer::flags> flags = {}) &;

        using view_impl::pos;
        void pos(pixel::point ps);

        using view_impl::size;
        void size(pixel::point sz);
        void size(scaler scl);

        using view_impl::title;
        void title(std::string_view val);

        using view_impl::fullscreen;
        void fullscreen(bool set);
    };
}