#pragma once

#include <type_traits>

// utility/metaprogramming.hpp:
// Template metaprogramming. Everything here resides in namespace hal::meta.

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

    namespace meta
    {
        // Check whether a type is present in a parameter pack.
        template <typename What, typename... Where>
        constexpr inline bool is_present_v { (std::is_same_v<What, Where> || ...) };

        // Get the index at which a type resides in a parameter pack that contains it.
        // This will report the index of the first occurrence.
        template <typename What, typename... Where>
            requires is_present_v<What, Where...>
        constexpr inline std::size_t index_of_v { detail::index_of<0, What, Where...>::value };

        // Get the type residing at an index in a parameter pack.
        template <std::size_t I, typename... Ts>
            requires(I < sizeof...(Ts))
        using type_at_t = detail::type_at<0, I, Ts...>::type;

        // A holder, of sorts, of a parameter pack.
        // Provides basic functionality.
        template <typename... Ts>
        struct type_list
        {
            constexpr static std::size_t size { sizeof...(Ts) };

            // Get the index of a type.
            template <typename T>
            constexpr static std::size_t find { index_of_v<T, Ts...> };

            // Get the type at index N.
            template <std::size_t N>
            using at = type_at_t<N, Ts...>;

            using front = at<0>;
            using back  = at<size - 1>;

            // Wrap the provided type list in a type that takes a parameter pack.
            // Hopefully, that's understandable to you, 'cause it sure ain't to me.
            template <template <typename...> typename T>
            using wrap = T<Ts...>;
        };
    }

    namespace detail
    {
        template <typename...>
        struct join;

        template <typename... ArgsL, typename... ArgsR>
        struct join<meta::type_list<ArgsL...>, meta::type_list<ArgsR...>>
        {
            using type = meta::type_list<ArgsL..., ArgsR...>;
        };
    }

    namespace meta
    {
        template <typename Pack1, typename Pack2>
        using join_t = detail::join<Pack1, Pack2>::type;

        // A neat way to get type information about a function.
        template <typename>
        struct func_info;

        template <typename Ret, typename... Args>
        struct func_info<Ret(Args...)>
        {
            using return_type = Ret;
            using args        = type_list<Args...>;
        };
    }

    namespace detail
    {
        template <typename T>
        struct is_correct_main : std::false_type
        {
        };

        template <>
        struct is_correct_main<int(int, char**)> : std::true_type
        {
        };
    }

    namespace meta
    {
        // Check whether the main function is correctly written out.
        // This is due to SDL's cross-platform hackery; on Windows, it redefines "main"
        // to be SDL_main, which hides away the OS' custom GUI application main function.
        template <auto MainFunc>
        constexpr inline bool is_correct_main_v = detail::is_correct_main<std::remove_pointer_t<decltype(MainFunc)>>::value;
    }
}