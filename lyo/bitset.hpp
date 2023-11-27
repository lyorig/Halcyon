#pragma once

#include <cassert>
#include <concepts>

#include "types.hpp"

/* bitset.hpp:
   A custom implementation of std::bitset. */

namespace lyo
{
    template <lyo::usize Size, std::unsigned_integral Storage_type = unsigned char>
    requires(Size > 0) class bitset
    {
      public:

        constexpr bitset() noexcept :
            m_storage {}
        {
        }

        constexpr bitset(bool initializer) noexcept :
            m_storage {}
        {
            // Set all bits to 1 if initializer == true by negating zero.
            if (initializer)
                for (Storage_type& unit : m_storage)
                    unit = ~static_cast<Storage_type>(0);
        }

        constexpr void set(lyo::usize pos) noexcept
        {
            m_storage[storage_index(pos)] |= shifted_one(pos);
        }

        constexpr void reset(lyo::usize pos) noexcept
        {
            m_storage[storage_index(pos)] &= ~shifted_one(pos);
        }

        constexpr void flip(lyo::usize pos) noexcept
        {
            m_storage[storage_index(pos)] ^= shifted_one(pos);
        }

        constexpr void clear() noexcept
        {
            for (Storage_type& unit : m_storage) unit = static_cast<Storage_type>(0);
        }

        constexpr lyo::usize size() const noexcept
        {
            return Size;
        }

        constexpr bool operator[](lyo::usize pos) const noexcept
        {
            assert(pos < Size);
            return static_cast<bool>((m_storage[storage_index(pos)] >> (pos % (sizeof(Storage_type) * 8))) & static_cast<Storage_type>(1));
        }

      private:

        // Convenience functions.
        constexpr static Storage_type shifted_one(lyo::usize pos) noexcept
        {
            return static_cast<Storage_type>(1) << (pos % (sizeof(Storage_type) * 8));
        }

        constexpr static lyo::usize storage_index(lyo::usize pos) noexcept
        {
            return static_cast<lyo::usize>(pos / (sizeof(Storage_type) * 8));
        }

        // You'll need eye bleach for this one. All you need to know is that it works.
        Storage_type m_storage[(Size + (sizeof(Storage_type) * 8) - 1) / (sizeof(Storage_type) * 8)];
    };
}  // namespace lyo