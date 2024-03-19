#pragma once

#include <halcyon/renderer.hpp>
#include <quest/types.hpp>

namespace quest
{
    // A camera that transforms object positions into rending coordinates.
    // The position represents the center of the viewport, not a corner.
     class camera
    {
    public:
        camera(const hal::renderer& rnd);

        void update(delta_t elapsed);

        // Detach the current target and move to a specified location.
        void move(const coord& pos, delta_t time);

        // Jump to a target and start following it.
        void target(const coord& pos, delta_t time);

        // Transform an object's position to rendering coordinates.
        // The renderer height parameter is used because the coordinate system used under
        // the hood has the origin at the top-left corner, meaning that the height goes "downward."
        hitbox transform(hitbox hbx, hal::pixel_point rsz) const;

    private:
        coord m_pos;

        std::variant<const coord*, coord> m_tgt;

        delta_t m_travelTime { 0.0 };
    };
}
