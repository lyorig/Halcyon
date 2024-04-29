#pragma once

#include <cstdint>
#include <span>
#include <string_view>

#include <halcyon/internal/sdl_object.hpp>

#include <halcyon/utility/pass_key.hpp>

// internal/outputter.hpp:
// The output version of the accessor class.

namespace hal
{
    class surface;

    namespace image
    {
        class context;
    }

    class outputter : sdl::object<SDL_RWops, ::SDL_RWclose> // Private inheritance by design.
    {
    public:
        outputter(std::string_view file);

        // std::as_bytes() returns a span of const bytes, which doesn't
        // work here, so both constructors exist independently.
        outputter(std::span<std::byte> data);
        outputter(std::span<std::uint8_t> data);

        unique_ptr use(pass_key<surface>);
        unique_ptr use(pass_key<image::context>);
    };

    [[nodiscard]] outputter output(std::string_view file);

    [[nodiscard]] outputter output(std::span<std::byte> data);
    [[nodiscard]] outputter output(std::span<std::uint8_t> data);
}