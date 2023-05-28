#pragma once

#include <type_traits>

/* concepts.hpp:
   An extension of STL concepts. */

namespace lyo
{
    template <typename T>
    concept arithmetic = std::is_arithmetic_v<T>;
}  // namespace lyo