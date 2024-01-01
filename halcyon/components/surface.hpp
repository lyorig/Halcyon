#pragma once

#include <SDL2/SDL_surface.h>

#include <halcyon/enums/blend.hpp>
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
    class blit;

    class image_loader;
    class video;
    class font;
    class surface;

    class surface : public sdl_object<SDL_Surface, &::SDL_FreeSurface>
    {
    public:
        // Create a sized surface.
        surface(const video& sys, pixel_size sz);

        // Get a resized copy of the surface. Useful for saving
        // memory after converting to a texture.
        surface resize(pixel_size sz);

        // Get a scaled copy of the surface. Useful for saving
        // memory after converting to a texture.
        surface resize(lyo::f64 scale);

        pixel_size size() const;

        // Get pixel at position.
        // This functionality is exclusive to surfaces, as textures
        // are extremely slow to retrieve pixel information.
        color operator[](pixel_pos coord) const;

        blend_mode get_blend() const;
        void       set_blend(blend_mode bm);

        // Public, but only available to the blit class.
        void internal_blit(const surface& to, const SDL_Rect* src, SDL_Rect* dst, lyo::pass_key<blit>) const;

    private:
        friend class image_loader;
        friend class font;

        // Special c-tor for factory classes.
        surface(SDL_Surface* surf);

        // Special c-tor for resizing.
        surface(pixel_size sz);

        Uint32 get_pixel(pixel_type x, pixel_type y) const;
    };

    LYO_TAG_TYPE(keep_dest);

    class blit : public drawer<surface, SDL::pixel_type, blit>
    {
    public:
        using drawer::drawer;

        void operator()(const surface& dst);

        // SDL's blitting function overwrites the destination rectangle.
        // This overload creates a copy to ensure it remains unchanged.
        void operator()(const surface& dst, keep_dest_tag) const;
    };

    template <typename T>
    class blend_lock
    {
    public:
        blend_lock(T& obj, blend_mode new_mode)
            : m_obj { obj }
            , m_old { obj.get_blend() }
        {
            obj.set_blend(new_mode);
        }

        ~blend_lock()
        {
            m_obj.set_blend(m_old);
        }

    private:
        T& m_obj;

        blend_mode m_old;
    };
} // namespace hal