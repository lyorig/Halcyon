#pragma once

#include <halcyon/debug.hpp>
#include <lyo/capsule.hpp>

namespace halcyon
{
    class engine;

    template <typename Type, lyo::function<void, Type*> Deleter>
    class sdl_object
    {
      public:

        sdl_object() noexcept = default;

        sdl_object(Type* object, const char* error_if_null) noexcept :
            m_object { object }
        {
            HALCYON_VERIFY(m_object != nullptr, error_if_null);
        }

        operator Type*() const noexcept
        {
            return m_object;
        }

      protected:

        lyo::capsule<Type, Deleter> m_object;
    };
}  // namespace halcyon