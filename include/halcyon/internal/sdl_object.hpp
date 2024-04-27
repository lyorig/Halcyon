#pragma once

#include <halcyon/debug.hpp>
#include <halcyon/types/numeric.hpp>
#include <memory>

namespace hal
{
    namespace sdl
    {
        template <typename Type, func_ptr<void, Type*> Deleter>
        class object
        {
        public:
            // A default constructor that doesn't perform a null check.
            object() = default;

            // A constructor that expects a valid object pointer.
            object(Type* object)
                : m_object { object }
            {
                HAL_ASSERT(valid(), debug::last_error());
            }

            // Return the underlying pointer to the object. Intended for internal
            // use, or for when you want to interface with SDL to use functions not
            // yet implemented in Halcyon.
            Type* ptr() const
            {
                return m_object.get();
            }

            // Check whether the object is valid and useable (a.k.a. non-null).
            bool valid() const
            {
                return ptr() != nullptr;
            }

            // Release (reset) the object.
            void release()
            {
                m_object.reset();
            }

        private:
            struct deleter
            {
                void operator()(Type* ptr)
                {
                    Deleter(ptr);
                }
            };

            std::unique_ptr<Type, deleter> m_object;
        };
    }
}