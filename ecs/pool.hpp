#pragma once

#include <lyo/buffer.hpp>

namespace ecs
{
    constexpr lyo::u8 COMPONENT_SPACE { 128 };

    class pool
    {
      public:

        using size_type = lyo::u64;

        pool(lyo::u16 elem_size) noexcept;

        void* operator[](size_type idx);

      private:

        lyo::buffer<std::byte> m_buffer;

        lyo::u16 m_elementSize;
    };
}  // namespace ecs