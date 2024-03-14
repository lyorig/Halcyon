#pragma once

// prefabs.hpp:
// Predefined entity classes, to be further adapted by both
// unique and non-unique variants.

#include <halcyon/texture.hpp>
#include <quest/camera.hpp>
#include <quest/types.hpp>

namespace quest::ent
{
    // Entity prefab namespace.
    namespace pfb
    {
        class character
        {
        public:
            character(hal::texture&& tex, coord spawn_point);

            void update(delta_t elapsed);
            void draw(hal::renderer& rnd, const camera& cam) const;

            coord hitbox() const;

            coord        pos, vel;
            hal::texture tex;

            lyo::f32 scale { 1.0 };

            direction dir { right };
            bool      moving { false };
        };

        static_assert(updateable<character> && drawable<character>);
    }
}