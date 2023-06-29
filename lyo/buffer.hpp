#include "capsule.hpp"

namespace lyo
{
    template <typename T>
    class buffer
    {
      public:

        using size_type = lyo::u32;

        buffer() noexcept :
            m_data { nullptr },
            size { 0 }
        {
        }

        buffer(size_type sz) noexcept :
            m_data { new T[sz] },
            size { sz }
        {
        }

        T& operator[](size_type pos) noexcept
        {
            assert(pos < size);

            return m_data[pos];
        }

      private:

        lyo::capsule<T[]> m_data;

      public:

        const size_type size;
    };
}  // namespace lyo