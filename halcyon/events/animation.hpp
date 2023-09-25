#pragma once

#include <lyo/timer.hpp>

/* animation.hpp:
   An experimental system for animations. */

namespace hal
{
    template <typename Animated, typename Functor>  // Add a "requires" clause later?
    class animation  // CTAD enabled.
    {
      public:

        constexpr animation(Animated& object, Functor animator) noexcept :
            m_animated { object },
            m_animator { animator }
        {
        }

        constexpr void update() noexcept
        {
            m_animator(m_animated, m_timer());
        }

        constexpr void set_animation(Functor animator) noexcept
        {
            m_animator = animator;
        }

        constexpr void reset() noexcept
        {
            m_timer.reset();
        }

      private:

        Animated& m_animated;
        Functor   m_animator;

        lyo::steady_timer m_timer;
    };
}