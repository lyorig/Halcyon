#pragma once

#include <halcyon/debug.hpp>
#include <lyo/capsule.hpp>

namespace hal
{
    template <typename Type, lyo::func_ptr<void, Type*> Deleter>
    class sdl_object
    {
      public:

        sdl_object() noexcept = default;

        sdl_object(Type* object) noexcept :
            m_object { object }
        {
            HAL_DEBUG_CHECK(m_object != nullptr, debug::sdl_error());
        }

        void reassign(Type* object) noexcept
        {
            HAL_DEBUG_ASSERT((m_object = object) != nullptr, debug::sdl_error());
        }

        Type* ptr() const noexcept
        {
            return m_object;
        }

      protected:

        lyo::capsule<Type, Deleter> m_object;
    };
}  // namespace hal