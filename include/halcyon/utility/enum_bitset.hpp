#pragma once

#include <utility>

#include <halcyon/utility/pass_key.hpp>

// utility/enum_bitset.hpp:
// Helper template class for "safe" enum flag values.

namespace hal
{
    // A bitset-like class for enums defined as bit masks (i.e. 0b1, 0b10, 0b100...).
    template <typename Enum, typename Value>
        requires(std::is_enum_v<Enum>)
    class enum_bitset
    {
    public:
        enum_bitset()
            : m_mask { 0 }
        {
        }

        enum_bitset(Value val)
            : m_mask { val }
        {
        }

        bool operator[](Enum e) const
        {
            if constexpr (std::is_pointer_v<Value>)
                return static_cast<bool>(m_mask[std::to_underlying(e)]);
            else
                return static_cast<bool>(m_mask & std::to_underlying(e));
        }

        Value mask() const
        {
            return m_mask;
        }

    private:
        Value m_mask;
    };
}