#pragma once

#include <span>
#include <string_view>

#include <SDL_rwops.h>

// internal/accessor.hpp:
// A wrapper of SDL_RWops that enables stuff to be loaded from not only
// files, but also "baked-in" data residing in memory.

namespace hal
{
    class accessor
    {
    public:
        accessor(SDL_RWops* ptr);
        ~accessor();

        SDL_RWops* get();

    private:
        SDL_RWops* m_ops;
    };

    // Load data from a file.
    accessor from_file(std::string_view file);

    // Load data from program memory.
    accessor from_memory(std::span<const std::byte> data);
    accessor from_memory(std::span<const std::uint8_t> data);
}