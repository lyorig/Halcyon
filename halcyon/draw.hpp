#pragma once

#include <halcyon/texture.hpp>
#include <optional>

namespace hal
{
    class texture;

    class drawer
    {
        public:

            drawer(lyo::pass_key<texture>, const texture& tex) noexcept;

            drawer& from(const pixel_area& src) noexcept;

            drawer& to(const coordinate& dst) noexcept;
            drawer& to(const world_area& dst) noexcept;

            drawer& scale(lyo::f64 scl) noexcept;

            drawer& rotate_around(const coordinate& center) noexcept;
            drawer& rotate(lyo::f64 angle) noexcept;

            drawer& anchor(anchor anch) noexcept;

            drawer& flip(enum flip f) noexcept;

            void operator()() const noexcept;

        private:

            using dest_rect = std::conditional_t<cfg::subpixel_drawing_precision, SDL_FRect, SDL_Rect>;

            constexpr world_area resolve_anchor() const noexcept;

            std::optional<SDL_FRect> m_dst; // where we're drawing
            std::optional<SDL_Rect> m_src; // which part of the texture we're drawing

            std::optional<SDL_FPoint> m_rot; // rotation center point

            lyo::f64 m_scale { 1.0 };
            lyo::f64 m_angle { 0.0 };

            hal::anchor m_anch { anchor::none };
            hal::flip m_flip { flip::none };

            const texture& m_tex;
    };
}