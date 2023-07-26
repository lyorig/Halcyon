#pragma once

#include <lyo/buffer.hpp>

namespace ecs
{
    // How many of one component there can be.
    constexpr lyo::u8 component_space { 128 };

    class pool
    {
        using element_size = lyo::u8;

      public:

        constexpr pool(lyo::usize elem_size) noexcept :
            m_buffer { elem_size * component_space },
            m_elementSize { static_cast<element_size>(elem_size) }
        {
            assert(elem_size <= std::numeric_limits<element_size>::max());
        }

        constexpr std::byte* operator[](lyo::usize idx) noexcept
        {
            return m_buffer.begin() + idx * m_elementSize;
        }

        constexpr operator bool() const noexcept
        {
            return m_buffer.data() != nullptr;
        }

      private:

        lyo::buffer<std::byte> m_buffer;

        const element_size m_elementSize;
    };
}  // namespace ecs