#pragma once

#include <halcyon/types/render.hpp>

namespace game
{
    class entity;

    class camera
    {
      public:

        camera() noexcept;
        camera(lyo::f64 speed) noexcept;

        void set_target(const entity& ent) noexcept;

        void update(lyo::f64 time) noexcept;

        hal::coordinate pos;

        lyo::f64 scale;
        lyo::f64 speed;

      private:

        const entity* m_target;
    };
}  // namespace game