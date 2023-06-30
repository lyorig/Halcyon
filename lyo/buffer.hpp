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
            m_size { 0 }
        {
        }

        buffer(size_type sz) noexcept :
            m_data { new T[sz] },
            m_size { sz }
        {
        }

        T& operator[](size_type pos) noexcept
        {
            assert(pos < m_size);

            return m_data[pos];
        }

        const T& operator[](size_type pos) const noexcept
        {
            assert(pos < m_size);

            return m_data[pos];
        }

        void resize(size_type sz) noexcept
        {
            m_data = new T[sz];
            m_size = sz;
        }

        size_type size() const noexcept
        {
            return m_size;
        }

      private:

        lyo::capsule<T[]> m_data;
        size_type         m_size;
    };
}  // namespace lyo