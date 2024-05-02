#pragma once

#include <span>

#include <halcyon/internal/sdl_object.hpp>
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

    extern template class sdl::object<SDL_RWops, ::SDL_RWclose>;

    // A proxy to various methods of accessing a file.
    // Loading functions "consume" this object, after which it is no longer useable.
    class accessor : public sdl::object<SDL_RWops, ::SDL_RWclose> // Private inheritance by design.
    {
    public:
        accessor(std::string_view path);
        accessor(std::span<const std::byte> data);

        SDL_RWops* get(pass_key<surface>) const;
        SDL_RWops* get(pass_key<image::context>) const;
        SDL_RWops* get(pass_key<ttf::context>) const;
    };

    [[nodiscard]] accessor access(std::string_view path);

    [[nodiscard]] accessor access(std::span<const std::byte> data);
    [[nodiscard]] accessor access(std::span<const std::uint8_t> data);

    // A proxy to various methods to outputting to a file.
    // Saving functions "consume" this object, after which it is no longer useable.
    class outputter : public sdl::object<SDL_RWops, ::SDL_RWclose> // Private inheritance by design.
    {
    public:
        outputter(std::string_view file);

        // std::as_bytes() returns a span of const bytes, which doesn't
        // work here, so both constructors exist independently.
        outputter(std::span<std::byte> data);
        outputter(std::span<std::uint8_t> data);

        SDL_RWops* get(pass_key<surface>) const;
        SDL_RWops* get(pass_key<image::context>) const;
    };

    [[nodiscard]] outputter output(std::string_view file);

    [[nodiscard]] outputter output(std::span<std::byte> data);
    [[nodiscard]] outputter output(std::span<std::uint8_t> data);
}