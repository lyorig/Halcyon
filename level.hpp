#pragma once

#include <lyo/capsule.hpp>
#include <vector>

#include "camera.hpp"
#include "ents/entity.hpp"

namespace game
{
    class level
    {
      public:

        template <typename Ent, typename... Args>
            requires std::is_base_of_v<entity, Ent>
        void spawn(Args... ctor_args) noexcept
        {
            m_ents.emplace_back(new Ent { ctor_args... });
        }

        void update(lyo::f64 time) noexcept;

      private:

        std::vector<lyo::capsule<entity>> m_ents;

        camera m_cam;
    };
}  // namespace game