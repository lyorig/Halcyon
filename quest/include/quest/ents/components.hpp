#pragma once

// prefabs.hpp:
// Predefined entity classes, to be further adapted by both
// unique and non-unique variants.

#include <halcyon/texture.hpp>
#include <quest/camera.hpp>
#include <quest/types.hpp>

// Entity component namespace.
namespace quest::ent::cmp
{
    struct scaleable
    {
        // An object's scale.
        using scale_t = lyo::f32;

        scaleable(hal::texture&& t, const coord& spawnpoint);

        hal::texture tex;
        coord        pos, size; // Cache the size for slightly faster drawing.

        void resize(scale_t scl);

        void draw(hal::renderer& rnd, const camera& cam) const;
    };
}