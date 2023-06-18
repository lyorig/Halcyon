#pragma once

#include <halcyon/types/render.hpp>

namespace hal
{
    class entity;  // Substitute with a base class with a position.
}

namespace hq
{
    class scene;

    class camera
    {
        friend class scene;

        using ent_base = hal::entity;

      public:

        void update() noexcept;

        void set_target(const ent_base& tgt) noexcept;

      private:

        camera() noexcept = default;

        const ent_base* m_target;

        hal::pixel_size m_pos;
    };
}  // namespace hq