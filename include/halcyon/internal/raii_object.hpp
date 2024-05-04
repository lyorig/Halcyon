#pragma once

#include <memory>

#include <halcyon/debug.hpp>

#include <halcyon/types/numeric.hpp>

// internal/raii_object.hpp:
// An SDL resource base class.

namespace hal
{
    namespace detail
    {
        template <typename Type, auto Deleter>
            requires std::is_invocable_v<decltype(Deleter), Type*>
        class raii_object
        {
        public:
            using value_type = Type;

            using pointer       = value_type*;
            using const_pointer = const value_type*;

        protected:
            // A default constructor that doesn't perform a null check.
            raii_object() = default;

            // A constructor that expects a valid object pointer.
            raii_object(pointer ptr)
                : m_object { ptr }
            {
                HAL_ASSERT(valid(), debug::last_error());
            }

            // Release the object.
            pointer release()
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
            pointer get() const
            {
                return m_object.get();
            }

            // Check whether the object is valid and useable (a.k.a. non-null).
            bool valid() const
            {
                return get() != nullptr;
            }

        private:
            struct deleter
            {
                void operator()(pointer ptr)
                {
                    static_cast<void>(Deleter(ptr));
                }
            };

            std::unique_ptr<value_type, deleter> m_object;
        };
    }
}