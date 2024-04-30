#pragma once

#include <cstdint>
#include <span>
#include <string_view>

#include <SDL_rwops.h>

#include <halcyon/internal/sdl_object.hpp>

#include <halcyon/utility/pass_key.hpp>

// internal/accessor.hpp:
// A wrapper of SDL_RWops that enables stuff to be loaded from not only
// files, but also "baked-in" data residing in memory. A destructor is not
// provided, because RW functions close it automatically.
// This has actually proved to be quite the challenge, since I also want to
// disallow the user from creating this class outside of its intended places.

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

    // A proxy to various methods of accessing a file. Only meant for use in
    // constructors and functions; creation anywhere else will cause a memory leak.
    class accessor : sdl::object<SDL_RWops, ::SDL_RWclose> // Private inheritance by design.
    {
    public:
        accessor(std::string_view path);
        accessor(std::span<const std::byte> data);

        accessor(const accessor&) = delete;
        accessor(accessor&&)      = default;

        SDL_RWops* get(pass_key<surface>);

        SDL_RWops* get(pass_key<image::context>) const;
        SDL_RWops* get(pass_key<ttf::context>);
    };

    [[nodiscard]] accessor access(std::string_view path);

    [[nodiscard]] accessor access(std::span<const std::byte> data);
    [[nodiscard]] accessor access(std::span<const std::uint8_t> data);
}