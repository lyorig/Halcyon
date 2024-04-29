#pragma once

#include <memory>

#include <halcyon/debug.hpp>

#include <halcyon/types/numeric.hpp>

namespace hal
{
    namespace sdl
    {
        template <typename Type, auto Deleter>
            requires std::is_invocable_v<decltype(Deleter), Type*>
        class object
        {
            struct deleter
            {
                void operator()(Type* ptr)
                {
                    static_cast<void>(Deleter(ptr));
                }
            };

        protected:
            // A default constructor that doesn't perform a null check.
            object() = default;

            // A constructor that expects a valid object pointer.
            object(Type* object)
                : m_object { object }
            {
                HAL_ASSERT(valid(), debug::last_error());
            }

        public:
            using unique_ptr = std::unique_ptr<Type, deleter>;

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

            // Move the unique_ptr holding the object.
            unique_ptr move()
            {
                return std::move(m_object);
            }

        private:
            unique_ptr m_object;
        };
    }
}