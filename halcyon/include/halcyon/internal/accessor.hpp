#pragma once

#include <SDL_rwops.h>
#include <span>
#include <string_view>

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

    accessor from_file(std::string_view file);
    accessor from_memory(std::span<const std::uint8_t> data);
}