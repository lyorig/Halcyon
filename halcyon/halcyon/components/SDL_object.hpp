#pragma once

#include <SDL2/SDL_error.h>
#include <halcyon/debug.hpp>
#include <memory>

namespace hal
{
    namespace sdl
    {
        template <typename Type, lyo::func_ptr<void, Type*> Deleter>
        struct deleter
        {
            void operator()(Type* ptr)
            {
                Deleter(ptr);
            }
        };

        template <typename Type, lyo::func_ptr<void, Type*> Deleter>
        class object
        {
        public:
            // A default constructor that doesn't check for null.
            object() = default;

            object(Type* object)
                : m_object { object }
            {
                HAL_DEBUG_ASSERT(m_object.get() != nullptr, ::SDL_GetError());
            }

            // Return the underlying pointer to the object. This is only for
            // when you want to interface with SDL to use functions not yet
            // implemented in Halcyon.
            Type* ptr() const
            {
                return m_object.get();
            }

            bool exists() const
            {
                return m_object.get() != nullptr;
            }

            // Release (reset) the object.
            void release()
            {
                m_object.reset();
            }

        protected:
            void reset(Type* object)
            {
                m_object.reset(object);
                HAL_DEBUG_ASSERT(m_object.get() != nullptr, ::SDL_GetError());
            }

        private:
            std::unique_ptr<Type, deleter<Type, Deleter>> m_object;
        };
    }
}