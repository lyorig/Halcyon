#include "capsule.hpp"

namespace lyo
{
    template <typename T>
    class buffer
    {
      public:

        buffer(lyo::u64 size) noexcept :
            m_data { new T[size] }
        {
        }

        operator T*() noexcept
        {
            return m_data;
        }

      private:

        lyo::capsule<T[]> m_data;
    };
}  // namespace lyo