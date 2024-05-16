#pragma once

#include <type_traits>

// utility/metaprogramming.hpp:
// Template metaprogramming. Everything here resides in namespace hal::meta.

namespace hal
{
    template <typename Return_type, typename... Args>
    using func_ptr = Return_type (*)(Args...);

    namespace detail
    {
        // Primary template.
        template <std::size_t I, std::size_t N, typename... Rest>
        struct at;

        // Not found.
        template <std::size_t I, std::size_t N, typename Current, typename... Rest>
        struct at<I, N, Current, Rest...> : at<I + 1, N, Rest...>
        {
        };

        // Found.
        template <std::size_t N, typename Current, typename... Rest>
        struct at<N, N, Current, Rest...>
        {
            using type = Current;
        };

        // Primary template.
        template <std::size_t I, typename What, typename... Rest>
        struct find;

        // Not found.
        template <std::size_t I, typename What, typename Current, typename... Ts>
        struct find<I, What, Current, Ts...> : find<I + 1, What, Ts...>
        {
        };

        // Found.
        template <std::size_t I, typename What, typename... Ts>
        struct find<I, What, What, Ts...>
        {
            constexpr static std::size_t value { I };
        };

        template <typename>
        struct array_size;

        template <typename T, std::size_t N>
        struct array_size<T[N]>
        {
            constexpr static std::size_t value { N };
        };
    }

    namespace meta
    {
        template <bool... Preds>
        constexpr inline bool all { (Preds && ...) };

        template <bool... Preds>
        constexpr inline bool any { (Preds || ...) };

        // Check whether a type is present in a parameter pack.
        template <typename What, typename... Where>
        constexpr inline bool is_present { (std::is_same_v<What, Where> || ...) };

        // Get the index at which a type resides in a parameter pack that contains it.
        // This will report the index of the first occurrence.
        template <typename What, typename... Where>
            requires is_present<What, Where...>
        constexpr inline std::size_t find { detail::find<0, What, Where...>::value };

        // Check if a type is "bare", as in, it is not a pointer, reference, and has no cv-qualifiers.
        template <typename T>
        constexpr inline bool is_bare { std::is_same_v<T, std::remove_cvref_t<std::remove_pointer_t<T>>> };

        // Get the type residing at an index in a parameter pack.
        template <std::size_t I, typename... Ts>
            requires(I < sizeof...(Ts))
        using at = detail::at<0, I, Ts...>::type;

        template <typename T>
        constexpr inline std::size_t array_size_v { detail::array_size<T>::value };

        // A holder, of sorts, of a parameter pack.
        // Provides basic functionality.
        template <typename... Ts>
        struct type_list
        {
            constexpr static std::size_t size { sizeof...(Ts) };

            // Get the index of a type.
            template <typename T>
            constexpr static std::size_t find { meta::find<T, Ts...> };

            // Get the type at index N.
            template <std::size_t N>
            using at = meta::at<N, Ts...>;

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
        using join = detail::join<Pack1, Pack2>::type;

        template <typename>
        struct func_info;

        // A neat way to get type information about a function.
        template <typename Ret, typename... Args>
        struct func_info<Ret(Args...)>
        {
            using function_type = Ret(Args...);
            using return_type   = Ret;
            using args          = type_list<Args...>;
        };

        // Adapter to get the info from a function pointer.
        template <typename Ret, typename... Args>
        struct func_info<func_ptr<Ret, Args...>> : func_info<Ret(Args...)>
        {
        };

        // Check whether the main function is correctly written out.
        // This is due to SDL's cross-platform hackery; on Windows, it redefines "main"
        // to be SDL_main, which hides away the OS' custom GUI application main function.
        // The problem is that this function has an explicit argc and argv parameter, so
        // while on Unix, you can have a main function with no parameters, this will not
        // build on Windows. Also note that, since SDL_main is a normal function that raturns int,
        // you must explicitly return an exit code - it doesn't have main's implicit "return 0".
        template <auto MainFunc>
        constexpr inline bool is_correct_main = std::is_same_v<decltype(MainFunc), func_ptr<int, int, char**>>;
    }
}