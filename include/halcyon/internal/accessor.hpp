#pragma once

#include <cstdint>
#include <span>

#include <string_view>

#include <SDL_rwops.h>

#include <lyo/pass_key.hpp>

// internal/accessor.hpp:
// A wrapper of SDL_RWops that enables stuff to be loaded from not only
// files, but also "baked-in" data residing in memory. A destructor is not
// provided, because RW functions close it automatically.

namespace hal
{
    class surface;

    namespace image
    {
        class context;
    }

    namespace ttf
    {
        class font;
    }

    class accessor
    {
    public:
        accessor(SDL_RWops* ptr);
        accessor(const accessor&) = delete;
        accessor(accessor&&)      = default;

        SDL_RWops* get(lyo::pass_key<surface>);
        SDL_RWops* get(lyo::pass_key<image::context>);
        SDL_RWops* get(lyo::pass_key<ttf::font>);

    private:
        SDL_RWops* m_ops;
    };

    accessor from_file(std::string_view file);
    accessor from_memory(std::span<const std::uint8_t> data);
}