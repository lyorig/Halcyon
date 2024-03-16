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
    struct drawable
    {
        drawable(hal::texture&& t);

        hal::texture tex;
        coord        pos;
    };

    struct scaleable : drawable
    {
        scaleable(hal::texture&& t);

        coord size;

        void resize(scale_t scl);
        void draw(hal::renderer& rnd, const camera& cam) const;
    };
}