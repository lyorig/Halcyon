#pragma once

#include <quest/camera.hpp>
#include <quest/types.hpp>

namespace quest::ent
{
    // This entity needs to be updated every frame. This does not mean that
    // it cannot change, but rather that it doesn't need to be done automatically.
    template <typename T>
    concept updateable = requires(T x, delta_t delta) { {x.update(delta) } -> std::same_as<void>; };

    // This entity has a texture and can be drawn to the screen.
    template <typename T>
    concept drawable = requires(T x, hal::renderer& rnd, const camera& cam) { {x.draw(rnd, cam)} -> std::same_as<void>; };

    // This entity can be collided with and has a hitbox.
    template <typename T>
    concept collideable = requires(T x) { {x.hitbox()} -> std::convertible_to<coord>; };

    // This entity can detect when it is "dead" and signal for its deletion.
    template <typename T>
    concept removable = requires(T x) { {x.can_remove()} -> std::same_as<bool>; };
}