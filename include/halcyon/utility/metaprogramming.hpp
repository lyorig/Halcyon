#pragma once

#include <type_traits>

// utility/metaprogramming.hpp:
// Template metaprogramming.

namespace hal
{
    namespace detail
    {
        // Primary template.
        template <std::size_t I, std::size_t N, typename... Rest>
        struct type_at;

        // Not found.
        template <std::size_t I, std::size_t N, typename Current, typename... Rest>
        struct type_at<I, N, Current, Rest...> : type_at<I + 1, N, Rest...>
        {
        };

        // Found.
        template <std::size_t N, typename Current, typename... Rest>
        struct type_at<N, N, Current, Rest...>
        {
            using type = Current;
        };

        // Primary template.
        template <std::size_t I, typename What, typename... Rest>
        struct index_of;

        // Not found.
        template <std::size_t I, typename What, typename Current, typename... Ts>
        struct index_of<I, What, Current, Ts...> : index_of<I + 1, What, Ts...>
        {
        };

        // Found.
        template <std::size_t I, typename What, typename... Ts>
        struct index_of<I, What, What, Ts...>
        {
            constexpr static std::size_t value = I;
        };
    }

    template <typename What, typename... Where>
    constexpr inline bool is_present_v = (std::is_same_v<What, Where> || ...);

    template <std::size_t I, typename... Ts>
        requires(I < sizeof...(Ts))
    using type_at_t = detail::type_at<0, I, Ts...>::type;

    template <typename What, typename... Where>
        requires is_present_v<What, Where...>
    constexpr inline std::size_t index_of_v = detail::index_of<0, What, Where...>::value;
}