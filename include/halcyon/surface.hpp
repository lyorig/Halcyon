#pragma once

#include <SDL_surface.h>

#include <halcyon/internal/accessor.hpp>
#include <halcyon/internal/drawer.hpp>
#include <halcyon/internal/sdl_object.hpp>

#include <halcyon/types/color.hpp>
#include <span>

// surface.hpp:
// A software bitmap, from which textures are created.
// Offers fast pixel retrieval and manipulation, which textures do not,
// but surface-on-surface blitting is slower due to not being GPU-accelerated.

namespace hal
{
    // Forward definitions for helper classes.
    class pixel_reference;
    class blitter;

    namespace image
    {
        class context;
    }

    namespace ttf
    {
        class font;
    }

    LYO_TAG(keep_dst);

    class surface : public sdl::object<SDL_Surface, &::SDL_FreeSurface>
    {
    public:
        class blend_lock
        {
        public:
            explicit blend_lock(surface& surf, blend_mode bm);
            ~blend_lock();

            void set(blend_mode bm);

        private:
            surface&   m_surf;
            blend_mode m_old;
        };

        // Create a sized surface.
        surface(pixel_point sz);

        // Load an image.
        surface(const image::context& auth, accessor data);

        // Render text using a font.
        surface(const ttf::font& font, std::string_view text, hal::color color = hal::palette::white);

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

        // Create a blitter.
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

        // Convert a color to a mapped value using this surface's pixel format.
        Uint32 mapped(color c) const;
    };

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

    class blitter : public detail::drawer<surface, sdl::pixel_t, const surface, blitter>
    {
    public:
        using drawer::drawer;

        void operator()();

        // SDL's blitting function overwrites the destination rectangle.
        // This overload creates a copy to ensure it remains unchanged.
        void operator()(keep_dst_tag) const;
    };
} // namespace hal