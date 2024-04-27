#pragma once

#include <span>
#include <string_view>

#include <halcyon/internal/sdl_rwops.hpp>

#include <halcyon/utility/pass_key.hpp>

// internal/outputter.hpp:
// The output version of the accessor class.

namespace hal
{
    class surface;

    class outputter : public sdl::rwops
    {
    public:
        outputter(std::string_view file);

        // std::as_bytes() returns a span of const bytes, which doesn't
        // work here, so both constructors exist independently.
        outputter(std::span<std::byte> data);
        outputter(std::span<std::uint8_t> data);

        SDL_RWops* get(pass_key<surface>);
    };

    [[nodiscard]] outputter output(std::string_view file);

    [[nodiscard]] outputter output(std::span<std::byte> data);
    [[nodiscard]] outputter output(std::span<std::uint8_t> data);
}