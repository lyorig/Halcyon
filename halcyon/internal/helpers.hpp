#pragma once

#include <lyo/concepts.hpp>
#include <halcyon/types/other.hpp>

namespace hal {

// OR together a set of values. Not sure what will happen
// if any of these have more than one bit set. Either way,
// you'll just be shooting yourself in the foot.
// The name stands for "initializer list to bit mask".
template <typename Cvt, typename T>
  constexpr Cvt il2bm(il<T> list) {
  Cvt mask{0};

  for (const auto value : list)
    mask |= static_cast<Cvt>(value);

  return mask;
}
}