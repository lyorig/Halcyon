#pragma once

#include <SDL2/SDL_surface.h>

#include <halcyon/internal/drawer.hpp>
#include <halcyon/types/color.hpp>
#include <lyo/pass_key.hpp>

#include <halcyon/components/sdl_object.hpp>

/* surface.hpp:
   A "software surface" used by SDL. The user should never
   directly create a surface, but instead use various loading
   functions of other classes, which are responsible for providing
   a valid SDL_Surface pointer. On top of staying true to the SDL object
   structure, another upside comes in the form of early error handling. */

namespace hal
{
    class image_loader;
    class window;
    class font;
    class surface;

    class blit : public drawer<surface, SDL::pixel_type, blit>
    {
    public:
        using drawer::drawer;
        void operator()(const surface& dst);
    };

    class surface : public sdl_object<SDL_Surface, &::SDL_FreeSurface>
    {
    public:
        // Create a sized surface.
        surface(const window& wnd, pixel_size sz);

        // Get a resized copy of the surface. Useful for saving
        // memory after converting to a texture.
        surface resize(pixel_size sz) const;

        // Get a scaled copy of the surface. Useful for saving
        // memory after converting to a texture.
        surface resize(lyo::f64 scale) const;

        void set_blend(SDL_BlendMode bm) const;

        pixel_size size() const;

        // Get pixel at position.
        // This functionality is exclusive to surfaces, as textures
        // are extremely slow to retrieve pixel information.
        color operator[](pixel_pos coord) const;

    private:
        friend class image_loader;
        friend class font;

        // Special c-tor for factory classes.
        surface(SDL_Surface* surf);

        // Special c-tor for resizing.
        surface(pixel_size sz);

        Uint32 get_pixel(pixel_type x, pixel_type y) const;
    };
} // namespace hal