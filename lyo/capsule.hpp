#pragma once

#include <utility>

#include "types.hpp"

/* capsule.hpp:
   Custom implementations of std::unique_ptr and std::shared_ptr. */

namespace lyo
{
    template <typename T>
    using capsule_base_type = std::remove_all_extents_t<T>;

    template <typename T>
    using capsule_deleter = lyo::function<void, capsule_base_type<T>*>;

    template <typename T>
    constexpr void default_delete(capsule_base_type<T>* ptr) noexcept
    {
        if (std::is_array_v<T>)
            delete[] ptr;

        else
            delete ptr;
    }

    template <typename Type, capsule_deleter<Type> Deleter = default_delete<Type>>
    class capsule
    {
      public:

        using base_type = capsule_base_type<Type>;

        constexpr capsule() noexcept :
            m_pointer { nullptr }
        {
        }

        constexpr capsule(std::nullptr_t) noexcept :
            m_pointer { nullptr }
        {
        }

        constexpr capsule(base_type* ptr) noexcept :
            m_pointer { ptr }
        {
        }

        constexpr capsule(const capsule& ref) = delete;

        constexpr capsule(capsule&& fwd) noexcept :
            m_pointer { std::move(fwd.m_pointer) }
        {
            fwd.m_pointer = nullptr;
        }

        constexpr capsule& operator=(const capsule& ref) = delete;

        constexpr capsule& operator=(capsule&& fwd) noexcept
        {
            if (m_pointer == fwd.m_pointer)
                return *this;

            m_pointer     = fwd.m_pointer;
            fwd.m_pointer = nullptr;

            return *this;
        }

        constexpr capsule& operator=(base_type* ptr) noexcept
        {
            if (m_pointer == ptr)
                return *this;

            this->destroy();

            m_pointer = ptr;

            return *this;
        }

        constexpr operator base_type*() const noexcept
        {
            return m_pointer;
        }

        constexpr base_type* operator->() const noexcept
        {
            return m_pointer;
        }

        constexpr base_type& operator*() const noexcept
        {
            return *m_pointer;
        }

        constexpr ~capsule()
        {
            this->destroy();
        }

      private:

        constexpr void destroy() const noexcept
        {
            if (m_pointer)
                Deleter(m_pointer);
        }

        base_type* m_pointer;
    };

    template <typename Type, capsule_deleter<Type> Deleter = default_delete<Type>>
    class shareable
    {
      public:

        using base_type    = capsule_base_type<Type>;
        using counter_type = lyo::u32;

        constexpr shareable() noexcept :
            m_pointer { nullptr },
            m_count { nullptr }
        {
        }

        constexpr shareable(std::nullptr_t) noexcept :
            m_pointer { nullptr },
            m_count { nullptr }
        {
        }

        constexpr shareable(base_type* pointer) noexcept :
            m_pointer { pointer },
            m_count { new counter_type { static_cast<counter_type>(1) } }
        {
        }

        constexpr shareable(const shareable& ref) noexcept :
            m_pointer { ref.m_pointer },
            m_count { ref.m_count }
        {
            if (ref.m_pointer)
                *m_count += 1;
        }

        constexpr shareable(shareable&& fwd) noexcept :
            m_pointer { fwd.m_pointer },
            m_count { fwd.m_count }
        {
            fwd.m_pointer = reinterpret_cast<base_type*>(fwd.m_count = nullptr);
        }

        constexpr shareable& operator=(const shareable& ref) noexcept
        {
            if (m_pointer == ref.m_pointer)
                return *this;

            this->destroy();

            m_pointer = ref.m_pointer;
            m_count   = ref.m_count;

            if (m_pointer)
                *m_count += 1;

            return *this;
        }

        constexpr shareable& operator=(shareable&& fwd) noexcept
        {
            if (m_pointer == fwd.m_pointer)
                return *this;

            this->destroy();

            m_pointer = std::move(fwd.m_pointer);
            m_count   = std::move(fwd.m_count);

            fwd.m_pointer = reinterpret_cast<base_type*>(fwd.m_count = nullptr);

            return *this;
        }

        constexpr shareable& operator=(base_type* ptr) noexcept
        {
            if (m_pointer == ptr)
                return *this;

            this->destroy();

            m_pointer = ptr;
            m_count   = ptr ? new counter_type { static_cast<counter_type>(1) } : nullptr;

            return *this;
        }

        constexpr counter_type use_count() const noexcept
        {
            return m_count ? *m_count : static_cast<counter_type>(0);
        }

        constexpr operator base_type*() const noexcept
        {
            return m_pointer;
        }

        constexpr base_type* operator->() const noexcept
        {
            return m_pointer;
        }

        constexpr base_type& operator*() const noexcept
        {
            return *m_pointer;
        }

        constexpr ~shareable()
        {
            this->destroy();
        }

      private:

        constexpr void destroy() noexcept
        {
            /* If m_pointer is a nullptr, m_count is either also
               a nullptr or shouldn't be decremented. Short circuiting
               ensures that it won't be accessed if the former check fails. */

            if (m_pointer && --(*m_count) == 0)
            {
                Deleter(m_pointer);
                delete m_count;
            }
        }

        base_type*    m_pointer;
        counter_type* m_count;
    };
}  // namespace lyo