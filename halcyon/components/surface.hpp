#pragma once

#include <SDL2/SDL_surface.h>

#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>

#include "sdl_object.hpp"

/* surface.hpp:
   A "software surface" used by SDL. The user should never
   directly create a surface, but instead use various loading
   functions of other classes, which are responsible for providing
   a valid SDL_Surface pointer. On top of staying true to the SDL object
   structure, another upside comes in the form of early error handling. */

namespace hal {
class image_loader;
class window;
class font;

class surface : public sdl_object<SDL_Surface, &::SDL_FreeSurface> {
public:
    // Create a sized surface.
    surface(window& wnd, pixel_size sz) noexcept;

    // Get a resized copy of the surface. Useful for saving
    // memory after converting to a texture.
    surface resize(pixel_size sz) const noexcept;

    // Get a scaled copy of the surface. Useful for saving
    // memory after converting to a texture.
    surface resize(lyo::f64 scale) const noexcept;

    pixel_size size() const noexcept;

    // Get pixel at position.
    // This functionality is exclusive to surfaces, as textures
    // are extremely slow to retrieve pixel information.
    color operator[](pixel_pos coord) const noexcept;

private:
    friend class image_loader;
    friend class font;

    // Special c-tor for factory classes
    surface(SDL_Surface* surf) noexcept;

    // Special c-tor for resizing
    surface(pixel_size sz) noexcept;

    void set_blend(SDL_BlendMode bm) const noexcept;

    Uint32 get_pixel(pixel_type x, pixel_type y) const noexcept;
};
} // namespace hal