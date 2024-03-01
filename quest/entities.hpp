#pragma once

#include "constants.hpp"
#include <halcyon/halcyon.hpp>
#include <string_view>

namespace quest
{
    // This entity needs to be updated every frame. This does not mean that
    // it cannot change, but rather that it doesn't need to be done automatically.
    template <typename T>
    concept updateable = requires(T x, lyo::f64 delta) { x.update(delta); };

    // This entity has a texture and can be drawn to the screen.
    template <typename T>
    concept drawable = requires(T x, hal::renderer& rnd) { x.draw(rnd); };

    using pos = hal::point<lyo::f64>;

    enum dir : lyo::i8
    {
        left  = -1,
        right = 1
    };

    // Entity base class. Contains common functionality.
    class entity
    {
    public:
        using ID = lyo::u32;

        constexpr entity(ID id)
            : m_id { id }
        {
        }

        constexpr ID id() const
        {
            return m_id;
        }

    private:
        const ID m_id;
    };

    class player : public entity
    {
    public:
        player(ID id, hal::texture&& tex);

        void update(lyo::f64 elapsed);
        void draw(hal::renderer& rnd) const;

        void set_direction(dir d);
        void set_moving(bool val);
        void set_pos(pos p);

    private:
        hal::texture m_tex;

        pos  m_pos, m_vel;
        dir  m_dir { dir::right };
        bool m_moving { false };
    };

    static_assert(updateable<player> && drawable<player>);

    struct bullet_shell : public entity
    {
        using entity::entity;

        void update(lyo::f64 elapsed);
        void draw(hal::renderer& rnd) const;
    };

    static_assert(updateable<bullet_shell> && drawable<bullet_shell>);

    struct light : public entity
    {
        using entity::entity;

        void draw(hal::renderer& rnd) const;
    };

    static_assert(drawable<light>);
}