#pragma once

#include <utility>

#include <halcyon/utility/concepts.hpp>
#include <halcyon/utility/pass_key.hpp>

// utility/enum_bitset.hpp:
// Helper template class for "safe" enum flag values.

namespace hal
{
    namespace detail
    {
        // OR together a set of values.
        // The name stands for "initializer list to bit mask".
        template <meta::arithmetic To, typename T>
            requires std::is_enum_v<T>
        constexpr To to_bitmask(std::initializer_list<T> list)
        {
            To mask { 0 };

            for (const T value : list)
                mask |= static_cast<To>(value);

            return mask;
        }

        template <meta::arithmetic To, typename T>
            requires std::is_enum_v<T>
        constexpr To to_bitset(std::initializer_list<T> list)
        {
            To mask { 0 };

            for (const T value : list)
                mask |= (1 << static_cast<To>(value));

            return mask;
        }

        template <typename Enum, typename Value>
            requires std::is_enum_v<Enum>
        class enum_bit_base
        {
        public:
            constexpr enum_bit_base()
                : m_mask { 0 }
            {
            }

            constexpr enum_bit_base(Value val)
                : m_mask { val }
            {
            }

        public:
            constexpr Value mask() const
            {
                return m_mask;
            }

        private:
            Value m_mask;
        };
    }

    // A bitset-like class for enums defined as bit masks (i.e. 0b1, 0b10, 0b100...).
    template <typename Enum, typename Value>
        requires std::is_enum_v<Enum>
    class enum_bitmask : public detail::enum_bit_base<Enum, Value>
    {
    private:
        using super = detail::enum_bit_base<Enum, Value>;

    public:
        using super::super;

        constexpr enum_bitmask(std::initializer_list<Enum> il)
            : super { detail::to_bitmask<Value>(il) }
        {
        }

        constexpr bool operator[](Enum e) const
        {
            if constexpr (std::is_pointer_v<Value>)
                return static_cast<bool>(super::mask()[std::to_underlying(e)]);
            else
                return static_cast<bool>(super::mask() & std::to_underlying(e));
        }
    };

    template <typename Enum, typename Value>
        requires std::is_enum_v<Enum>
    class enum_bitset : public detail::enum_bit_base<Enum, Value>
    {
    private:
        using super = detail::enum_bit_base<Enum, Value>;

    public:
        using super::super;

        constexpr enum_bitset(std::initializer_list<Enum> il)
            : super { detail::to_bitset<Value>(il) }
        {
        }

        constexpr bool operator[](Enum e) const
        {
            return static_cast<bool>(super::mask() & static_cast<Value>(e));
        }
    };
}