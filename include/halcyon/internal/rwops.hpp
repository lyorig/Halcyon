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

    // A proxy to various methods of accessing a file.
    // Loading functions "consume" this object, after which it is no longer useable.
    class accessor : public detail::raii_object<SDL_RWops, ::SDL_RWclose>
    {
    public:
        accessor(std::string_view path);
        accessor(std::span<const std::byte> data);

        // Functions that use get seek the RWops back where they started.
        SDL_RWops* get(pass_key<image::context>) const;

        // Getter functions call release(), so the class gets "consumed".
        SDL_RWops* use(pass_key<surface>);
        SDL_RWops* use(pass_key<image::context>);
        SDL_RWops* use(pass_key<ttf::context>);
    };

    [[nodiscard]] accessor access(std::string_view path);

    [[nodiscard]] accessor access(std::span<const std::byte> data);
    [[nodiscard]] accessor access(std::span<const std::uint8_t> data);

    // A proxy to various methods to outputting to a file.
    // Saving functions "consume" this object, after which it is no longer useable.
    class outputter : public detail::raii_object<SDL_RWops, ::SDL_RWclose>
    {
    public:
        outputter(std::string_view file);

        // std::as_bytes() returns a span of const bytes, which doesn't
        // work here, so both constructors exist independently.
        outputter(std::span<std::byte> data);
        outputter(std::span<std::uint8_t> data);

        // use() functions call release(), so the class gets "consumed".

        SDL_RWops* use(pass_key<surface>);
        SDL_RWops* use(pass_key<image::context>);
    };

    [[nodiscard]] outputter output(std::string_view file);

    [[nodiscard]] outputter output(std::span<std::byte> data);
    [[nodiscard]] outputter output(std::span<std::uint8_t> data);
}