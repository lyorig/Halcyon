#pragma once

#include <memory>

#include <halcyon/debug.hpp>

#include <halcyon/types/numeric.hpp>

namespace hal
{
    namespace detail
    {
        template <typename Type, auto Deleter>
            requires std::is_invocable_v<decltype(Deleter), Type*>
        class raii_object
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
            raii_object() = default;

            // A constructor that expects a valid object pointer.
            raii_object(Type* object)
                : m_object { object }
            {
                HAL_ASSERT(valid(), debug::last_error());
            }

            // Release the object.
            Type* release()
            {
                return m_object.release();
            }

        public:
            // Free the object.
            void reset()
            {
                return m_object.reset();
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

        private:
            std::unique_ptr<Type, deleter> m_object;
        };
    }
}