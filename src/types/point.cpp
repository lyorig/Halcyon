#include <halcyon/types/point.hpp>

namespace hal
{
    template struct point<std::int8_t>;
    template struct point<std::int16_t>;
    template struct point<std::int32_t>;
    template struct point<std::int64_t>;

    template struct point<std::uint8_t>;
    template struct point<std::uint16_t>;
    template struct point<std::uint32_t>;
    template struct point<std::uint64_t>;

    template struct point<float>;
    template struct point<double>;
}