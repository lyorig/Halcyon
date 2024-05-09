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
            constexpr static std::size_t value { I };
        };
    }

    template <typename What, typename... Where>
    constexpr inline bool is_present_v { (std::is_same_v<What, Where> || ...) };

    // Get the type residing at an index in a parameter pack.
    template <std::size_t I, typename... Ts>
        requires(I < sizeof...(Ts))
    using type_at_t = detail::type_at<0, I, Ts...>::type;

    // Get the index at which a type resides in a parameter pack that contains it.
    // This will report the index of the first occurrence.
    template <typename What, typename... Where>
        requires is_present_v<What, Where...>
    constexpr inline std::size_t index_of_v { detail::index_of<0, What, Where...>::value };

    // A holder, of sorts, of a parameter pack.
    // Provides basic functionality.
    template <typename... Ts>
    struct type_list
    {
        constexpr static std::size_t size { sizeof...(Ts) };

        template <typename T>
        constexpr static std::size_t index_of { index_of_v<T, Ts...> };

        template <std::size_t N>
        using at_index = type_at_t<N, Ts...>;

        // Wrap the provided type list in a type that takes a parameter pack.
        // Hopefully, that's understandable to you, 'cause it sure ain't to me.
        template <template <typename...> typename T>
        using wrap = T<Ts...>;
    };

    template <typename>
    struct function_info;

    template <typename Ret, typename... Args>
    struct function_info<Ret(Args...)>
    {
        using return_type = Ret;
        using arguments   = type_list<Args...>;
    };
}