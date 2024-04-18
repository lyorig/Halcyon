#pragma once

#include <cstdint>
#include <span>
#include <string_view>

#include <SDL_rwops.h>

#include <halcyon/utility/pass_key.hpp>

// internal/accessor.hpp:
// A wrapper of SDL_RWops that enables stuff to be loaded from not only
// files, but also "baked-in" data residing in memory. A destructor is not
// provided, because RW functions close it automatically.

namespace hal
{
    class surface;

    namespace ttf
    {
        class font;
    }

    // A proxy to various methods of accessing a file. Only meant for use in
    // constructors and functions; creation anywhere else will cause a memory leak.
    class accessor
    {
    public:
        // Non-copyable, only moveable to accomodate some
        // convenience factory functions.
        accessor(const accessor&) = delete;
        accessor(accessor&&)      = default;

        friend accessor load(std::string_view file);
        friend accessor load(std::span<const std::uint8_t> data);
        friend accessor load(std::span<const std::byte> data);

        SDL_RWops* get(pass_key<surface>);
        SDL_RWops* get(pass_key<ttf::font>);

    private:
        accessor(SDL_RWops* ptr);

        SDL_RWops* m_ops;
    };

    // Load data from a file.
    [[nodiscard]] accessor load(std::string_view file);

    // Load data from program memory.
    [[nodiscard]] accessor load(std::span<const std::uint8_t> data);
    [[nodiscard]] accessor load(std::span<const std::byte> data);
}