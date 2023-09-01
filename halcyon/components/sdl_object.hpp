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
            HAL_DEBUG_CHECK(m_object.get() != nullptr, ::SDL_GetError());
        }

        // Return the underlying pointer to the object. This is only for
        // when you want to interface with SDL to use functions not yet
        // implemented in Halcyon.
        Type* ptr() const noexcept
        {
            return m_object.get();
        }

      protected:

        void reassign(Type* object) noexcept
        {
            HAL_DEBUG_ASSERT((m_object.reset(object)).get() != nullptr, ::SDL_GetError());
        }

        lyo::capsule<Type, Deleter> m_object;
    };
}  // namespace hal