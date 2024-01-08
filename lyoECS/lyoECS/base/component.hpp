#pragma once

#include <lyoSTL/concepts.hpp>
#include <lyoSTL/types.hpp>

namespace lyo
{
    namespace ECS
    {
        template <lyo::arithmetic T>
        constexpr inline T invalid = std::numeric_limits<T>::max();

        namespace comp
        {
            using ID    = lyo::u8;
            using index = lyo::u16;

            enum : index
            {
                invalid_index = invalid<index>
            };
        }

        template <typename T, comp::index Size>
            requires(Size > 0)
        struct info
        {
            using type = T;

            consteval static comp::index amount() { return Size; }
            consteval static std::size_t size_bytes() { return Size * sizeof(T); }
        };

        template <typename C, typename... Is>
        using associated_type_t = lyo::type_at_index<lyo::index_v<C, typename Is::type...>, Is...>;

        template <typename... Is>
        constexpr inline std::size_t total_amount_v { (Is::amount() + ...) };

        template <typename... Is>
        constexpr inline std::size_t total_size_bytes_v { (Is::size_bytes() + ...) };

        template <typename I, typename... Is>
        struct index_begin;

        template <typename I, typename I2, typename... Is>
        struct index_begin<I, I2, Is...> : std::integral_constant<std::size_t, I2::amount() + index_begin<I, Is...>::value>
        {
        };

        template <typename I, typename... Is>
        struct index_begin<I, I, Is...> : std::integral_constant<std::size_t, 0>
        {
        };

        template <typename I, typename... Is>
        struct byte_begin;

        template <typename I, typename I2, typename... Is>
        struct byte_begin<I, I2, Is...> : std::integral_constant<std::size_t, I2::size_bytes() + byte_begin<I, Is...>::value>
        {
        };

        template <typename I, typename... Is>
        struct byte_begin<I, I, Is...> : std::integral_constant<std::size_t, 0>
        {
        };

        template <typename C, typename... Is>
        constexpr inline std::size_t index_begin_v = index_begin<associated_type_t<C, Is...>, Is...>::value;

        template <typename C, typename... Is>
        constexpr inline std::size_t byte_begin_v = byte_begin<associated_type_t<C, Is...>, Is...>::value;

        // Checks if a component is present in a pack of infos.
        template <typename C, typename... Is>
        concept in_infos = lyo::is_present_v<C, typename Is::type...>;
    }
}