#pragma once

#include <SDL_surface.h>

#include <halcyon/enums/blend.hpp>
#include <halcyon/internal/drawer.hpp>
#include <halcyon/internal/sdl_object.hpp>
#include <halcyon/types/color.hpp>
#include <lyo/pass_key.hpp>
#include <mdspan>

// surface.hpp:
// A "software surface" used by SDL. The user should never
// directly create a surface, but instead use various loading
// functions of other classes, which are responsible for providing
// a valid SDL_Surface pointer. On top of staying true to the SDL object
// structure, another upside comes in the form of early error handling.

namespace hal
{
    // Forward defs for external constructor classes.
    class image_loader;
    class font;

    // Ditto for helper classes.
    class blitter;

    LYO_TAG(keep_dst);

    class surface : public sdl::object<SDL_Surface, &::SDL_FreeSurface>
    {
        // A reference to a pixel in a surface for easy access and modification.
        class pixel_reference
        {
        public:
            pixel_reference(void* pixels, int pitch, const SDL_PixelFormat* fmt, pixel_point pos);

            operator color() const;

            pixel_reference& operator=(color c);

        private:
            // Retrieve the color in a mapped format.
            Uint32 get() const;

            // Set the color to a mapped value.
            void set(Uint32 mv);

            std::byte*             m_ptr; // A pointer to the current color
            const SDL_PixelFormat* m_fmt;
        };

    public:
        // Create a sized surface.
        surface(pixel_point sz);

        // Create a surface from an array of colors.
        // If you are not yet accustomed to std::mdspan, the dimensional extents are in [Y, X] format.
        // Either that, or I'm not understanding the class correctly. Womp womp.
        template <typename Extents>
        surface(std::mdspan<const hal::color, Extents> colors)
            : surface { { static_cast<pixel_t>(colors.extent(1)), static_cast<pixel_t>(colors.extent(0)) } }
        {
            for (pixel_point pos { 0, 0 }; static_cast<std::size_t>(pos.y) != colors.extent(0); ++pos.y, pos.x = 0)
            {
                for (; static_cast<std::size_t>(pos.x) != colors.extent(1); ++pos.x)
                {
                    operator[](pos) = colors[pos.y, pos.x];
                }
            }
        }

        // Special c-tor for factory classes.
        surface(SDL_Surface* surf, lyo::pass_key<image_loader>);
        surface(SDL_Surface* surf, lyo::pass_key<font>);

        // Get a resized copy of the surface. Useful for saving
        // memory after converting to a texture.
        surface resize(pixel_point sz);
        surface resize(lyo::f64 scale);

        void fill(color clr);
        void fill_rect(const sdl::pixel_rect& area, color clr);
        void fill_rects(const std::span<const sdl::pixel_rect>& areas, color clr);

        pixel_point size() const;

        // Get pixel at position.
        // This functionality is exclusive to surfaces, as textures
        // are extremely slow to retrieve pixel information.
        pixel_reference operator[](const pixel_point& pos) const;

        blend_mode blend() const;
        void       blend(blend_mode bm);

        [[nodiscard]] blitter blit(surface& dst) const;

        // Public, but only available to the blit class.
        void internal_blit(const surface& to, const sdl::pixel_rect* src, sdl::pixel_rect* dst, lyo::pass_key<blitter>) const;

    private:
        // I'd rather use the pixel format enum, but SDL uses an integer
        // in pretty much every API function, so I'll save myself the hassle.
        surface(pixel_point sz, int depth, Uint32 format);

        // Construct a new surface from an existing one's pixel format.
        // Used for resizing.
        surface(pixel_point sz, const SDL_PixelFormat* fmt);

        Uint32 mapped(color c) const;
    };

    class blitter : public drawer<surface, sdl::pixel_t, const surface, blitter>
    {
    public:
        using drawer::drawer;

        void operator()();

        // SDL's blitting function overwrites the destination rectangle.
        // This overload creates a copy to ensure it remains unchanged.
        void operator()(keep_dst_tag) const;
    };

    template <typename T>
    class blend_lock
    {
    public:
        blend_lock(T& obj, blend_mode new_mode)
            : m_obj { obj }
            , m_old { obj.blend() }
        {
            obj.blend(new_mode);
        }

        ~blend_lock()
        {
            m_obj.blend(m_old);
        }

    private:
        T& m_obj;

        blend_mode m_old;
    };
} // namespace hal