#pragma once

#include <span>

#include <SDL_rwops.h>

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/utility/pass_key.hpp>

namespace hal
{
    class surface;

    namespace image
    {
        class context;
    }

    namespace ttf
    {
        class context;
    }

    namespace detail
    {
        template <typename T>
        concept rwops_string = std::is_assignable_v<std::string_view, T>;

        static_assert(
            rwops_string<const char*>
            && rwops_string<const char[10]>
            && rwops_string<std::string>
            && rwops_string<std::string_view>
            && !rwops_string<std::span<const std::uint8_t>>);

        template <typename T>
        concept accessor_array = !rwops_string<T>
            && requires(const T& x) {std::data(x); std::size(x); typename std::enable_if_t<sizeof(std::remove_pointer_t<decltype(std::data(x))>) == 1>; };

        static_assert(
            accessor_array<const std::byte[10]>
            && accessor_array<std::uint8_t[512]>
            && accessor_array<std::span<const char>> && !accessor_array<std::span<const int>>);

        template <typename T>
        concept outputter_array = accessor_array<T>
            && requires(T& x) { *std::data(x) = std::declval<std::remove_pointer_t<decltype(std::data(x))>>(); };

        static_assert(outputter_array<std::uint8_t[10]>
            && !outputter_array<const std::uint8_t[10]>);

        template <typename T>
        const char* string_data(const T& obj)
        {
            if constexpr (std::is_same_v<meta::remove_const_pointer<T>, char*>)
                return obj;

            else
                return std::data(obj);
        }
    }

    // A proxy to various methods of accessing a file.
    // Loading functions "consume" this object, after which it is no longer useable.
    class accessor : public detail::raii_object<SDL_RWops, ::SDL_RWclose>
    {
    public:
        // Access a string. This accepts anything that can be assigned (not explicitly) to a std::string_view.
        template <detail::rwops_string T>
        accessor(const T& path)
            : raii_object { ::SDL_RWFromFile(detail::string_data(path), "r") }
        {
        }

        // Access an array.
        // This accepts any container that cannot be assigned to a std::string_view.
        template <detail::accessor_array T>
        accessor(const T& buffer)
            requires(sizeof(std::remove_pointer_t<decltype(std::data(buffer))>) == 1)
            : raii_object { ::SDL_RWFromConstMem(std::data(buffer), std::size(buffer)) }
        {
        }

        // Functions that use get seek the RWops back where they started.
        SDL_RWops* get(pass_key<image::context>) const;

        // Getter functions call release(), so the class gets "consumed".
        SDL_RWops* use(pass_key<surface>);
        SDL_RWops* use(pass_key<image::context>);
        SDL_RWops* use(pass_key<ttf::context>);
    };

    // A proxy to various methods to outputting to a file.
    // Saving functions "consume" this object, after which it is no longer useable.
    class outputter : public detail::raii_object<SDL_RWops, ::SDL_RWclose>
    {
    public:
        template <detail::rwops_string T>
        outputter(const T& path)
            : raii_object { ::SDL_RWFromFile(detail::string_data(path), "w") }
        {
        }

        template <detail::outputter_array T>
        outputter(T& buffer)
            : raii_object { ::SDL_RWFromMem(std::data(buffer), std::size(buffer)) }
        {
        }

        // std::as_bytes() returns a span of const bytes, which doesn't
        // work here, so both constructors exist independently.
        outputter(std::span<std::byte> data);
        outputter(std::span<std::uint8_t> data);

        // use() functions call release(), so the class gets "consumed".

        SDL_RWops* use(pass_key<surface>);
        SDL_RWops* use(pass_key<image::context>);
    };
}