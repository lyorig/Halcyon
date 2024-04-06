#pragma once

#include <SDL_rwops.h>
#include <cstdint>
#include <span>
#include <string_view>

namespace hal
{
    namespace detail
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
    }

    detail::accessor from_file(std::string_view file);
    detail::accessor from_memory(std::span<const std::uint8_t> data);
}