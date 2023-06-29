#pragma once

#include <cmath>
#include <cstdint>
#include <type_traits>

#define MAYBE_EMPTY [[no_unique_address]]

/* types.hpp:
   Various type aliases and related stuff. */

namespace lyo
{
    constexpr bool fast_types { true };  // Use potentially faster types (e.g. int_fastN_t instead of intN_t).

    template <typename Normal_type, typename Fast_type>
    using CFT = std::conditional_t<fast_types, Fast_type, Normal_type>;  // Conditional Fast Type.

    using i8  = CFT<std::int8_t, std::int_fast8_t>;
    using i16 = CFT<std::int16_t, std::int_fast16_t>;
    using i32 = CFT<std::int32_t, std::int_fast32_t>;
    using i64 = CFT<std::int64_t, std::int_fast64_t>;

    using u8  = std::make_unsigned_t<i8>;
    using u16 = std::make_unsigned_t<i16>;
    using u32 = std::make_unsigned_t<i32>;
    using u64 = std::make_unsigned_t<i64>;

    using f32 = CFT<float, std::float_t>;
    using f64 = CFT<double, std::double_t>;

    template <typename Return_type, typename... Args>
    using func_ptr = Return_type (*)(Args...);
}  // namespace lyo