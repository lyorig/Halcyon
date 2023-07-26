#pragma once

#include <halcyon/types/render.hpp>
#include <lyo/timer.hpp>

namespace game
{
    class scene;

    class entity
    {
      public:

        entity(const hal::coordinate& pos) noexcept;

        // Every derived class supplies its own "marked for death" condition.
        virtual bool can_remove() const noexcept = 0;

        virtual void update(lyo::f64 time) noexcept = 0;

        virtual ~entity() = 0;

      private:

        hal::coordinate m_pos;
    };

    class d1 : public entity
    {
      public:

        using entity::entity;

        virtual bool can_remove() const noexcept;
        virtual void update(lyo::f64 time) noexcept;

        ~d1();
    };

    class d2 : public entity
    {
      public:

        using entity::entity;

        virtual bool can_remove() const noexcept;
        virtual void update(lyo::f64 time) noexcept;

        ~d2();

      private:

        lyo::precise_timer m_timer;
    };
}  // namespace game