#pragma once

#include <halcyon/halcyon.hpp>

#include <quest/camera.hpp>
#include <quest/types.hpp>

#include <string_view>

namespace quest
{
    // This entity needs to be updated every frame. This does not mean that
    // it cannot change, but rather that it doesn't need to be done automatically.
    template <typename T>
    concept updateable = requires(T x, delta_t delta) { x.update(delta); };

    // This entity has a texture and can be drawn to the screen.
    template <typename T>
    concept drawable = requires(T x, hal::renderer& rnd, const camera& cam) { x.draw(rnd, cam); };

    // This entity can detect when it is dead and can be removed.
    template <typename T>
    concept removable = requires(T x) { x.can_remove(); };

    // Entity base class. Contains common functionality.
    class entity
    {
    public:
        using ID = lyo::u32;

        entity(ID id);

        ID id() const;

    private:
        ID m_id;
    };

    class character : public entity
    {
    public:
        character(entity::ID id, hal::texture&& tex, coord spawn_point);

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