#pragma once

#include <SDL.h>

#include <halcyon/debug.hpp>

// internal/raii_object.hpp:
// An SDL resource base class.

namespace hal
{
    namespace detail
    {
        template <typename Type>
        class view_base
        {
        public:
            using value_type = Type;

            using pointer       = value_type*;
            using const_pointer = const value_type*;

            // Return the underlying pointer to the object. Intended for internal
            // use, or for when you want to interface with SDL to use functions not
            // yet implemented in Halcyon.
            pointer get() const
            {
                return m_ptr;
            }

            // Check whether the object is valid and useable (a.k.a. non-null).
            bool valid() const
            {
                return get() != nullptr;
            }

        protected:
            view_base()
                : m_ptr { nullptr }
            {
            }

            view_base(std::nullptr_t) = delete;

            view_base(Type* ptr)
                : m_ptr { ptr }
            {
                HAL_ASSERT(valid(), debug::last_error());
            }

            view_base(const view_base&)            = delete;
            view_base& operator=(const view_base&) = delete;

            view_base(view_base&& o)
                : m_ptr { o.m_ptr }
            {
                o.m_ptr = nullptr;
            }

            view_base& operator=(view_base&& o)
            {
                m_ptr   = o.m_ptr;
                o.m_ptr = nullptr;
            }

            Type* m_ptr;
        };

        // A view is essentially a non-owning SDL object.
        // It contains non-modifying/querying member functions,
        // and is then extended by raii_object, which adds modifiers and
        // a destructor that disposes of the contained pointer.
        template <typename Type>
        class view_impl;

        template <typename Type, auto Deleter>
            requires std::is_invocable_v<decltype(Deleter), Type*>
        class raii_object : public view_impl<Type>
        {
            using super = view_impl<Type>;

        public:
            raii_object() = default;

            raii_object(super::pointer p)
                : super { p }
            {
            }

            raii_object(std::nullptr_t) = delete;

            raii_object(const raii_object&)            = delete;
            raii_object& operator=(const raii_object&) = delete;

            raii_object(raii_object&& o)
                : super { std::move(o) }
            {
            }

            raii_object& operator=(raii_object&& o)
            {
                super::m_ptr = o.m_ptr;
                o.m_ptr      = nullptr;

                return *this;
            }

            ~raii_object()
            {
                reset();
            }

            // Free the object.
            void reset()
            {
                if (view_impl<Type>::m_ptr)
                    Deleter(view_impl<Type>::m_ptr);

                view_impl<Type>::m_ptr = nullptr;
            }

        protected:
            // Release the object.
            view_impl<Type>::pointer release()
            {
                auto ret { view_impl<Type>::m_ptr };

                view_impl<Type>::m_ptr = nullptr;

                return ret;
            }
        };
    }

    namespace view
    {
        using window   = detail::view_impl<SDL_Window>;
        using renderer = detail::view_impl<SDL_Renderer>;

        using surface = detail::view_impl<SDL_Surface>;

        using texture = detail::view_impl<SDL_Texture>;

        using string = detail::view_impl<char>;
    }
}