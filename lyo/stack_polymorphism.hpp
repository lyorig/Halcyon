#pragma once

#include <algorithm>
#include <array>
#include <lyo/concepts.hpp>

// stack_polymorphism.hpp:
// An alternative to unique_ptr polymorphism which doesn't use the heap.

namespace lyo
{
    // Tag struct for constructor template deduction.
    template <typename T>
    struct init_with
    {
    };

    template <typename Base, typename... Derived>
        requires(std::is_base_of_v<Base, Derived> && ...)
    class stack_pmr
    {
    public:
        constexpr stack_pmr()
        {
            invalidate();
        };

        template <one_of<Derived...> D, typename... Args>
        constexpr stack_pmr([[maybe_unused]] init_with<D> tag, Args&&... args)
        {
            reset<D>(std::forward<Args>(args)...);
        };

        constexpr ~stack_pmr()
        {
            reset();
        }

        constexpr void reset()
            requires(std::has_virtual_destructor_v<Base>)
        {
            if (valid())
            {
                get()->~Base();
                invalidate();
            }
        }

        template <one_of<Base, Derived...> D, typename... Args>
        constexpr void reset(Args&&... args)
        {
            reset();
            new (reinterpret_cast<D*>(m_data.data())) D { std::forward<Args>(args)... };
        }

        constexpr Base* get()
        {
            return reinterpret_cast<Base*>(m_data.data());
        }

        constexpr const Base* get() const
        {
            return reinterpret_cast<const Base*>(m_data.data());
        }

        constexpr Base* operator->()
        {
            return get();
        }

        constexpr const Base* operator->() const
        {
            return get();
        }

        constexpr Base& operator*()
        {
            return *get();
        }

        constexpr const Base& operator*() const
        {
            return *get();
        }

    private:
        constexpr void invalidate()
        {
            reinterpret_cast<void*&>(m_data.front()) = 0;
        }

        constexpr bool valid() const
        {
            return reinterpret_cast<const void*>(m_data.front()) != 0;
        }

        std::array<std::byte, std::max({ std::size_t { 0 }, sizeof(Derived)... })> m_data;
    };
}