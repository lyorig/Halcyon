#pragma once

#include <utility>

#include <halcyon/utility/pass_key.hpp>

// enum_bitset.hpp:
// Helper template class for "safe" enum flag values.

namespace hal
{
    namespace detail
    {
        template <typename Enum, typename Value>
            requires(std::is_enum_v<Enum>)
        class enum_bitset
        {
        public:
            enum_bitset(Value val)
                : m_value { val }
            {
            }

            bool operator[](Enum e) const
            {
                if constexpr (std::is_pointer_v<Value>)
                    return static_cast<bool>(m_value[std::to_underlying(e)]);
                else
                    return static_cast<bool>(m_value & std::to_underlying(e));
            }

        private:
            Value m_value;
        };
    }
}