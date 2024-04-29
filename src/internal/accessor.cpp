#include <halcyon/debug.hpp>
#include <halcyon/internal/accessor.hpp>

using namespace hal;

accessor::accessor(std::string_view path)
    : object { ::SDL_RWFromFile(path.data(), "r") }
{
}

accessor::accessor(std::span<const std::byte> data)
    : object { ::SDL_RWFromConstMem(data.data(), data.size_bytes()) }
{
}

SDL_RWops* accessor::get(pass_key<image::context>) const
{
    return ptr();
}

accessor::unique_ptr accessor::use(pass_key<image::context>)
{
    return move();
}

accessor::unique_ptr accessor::use(pass_key<ttf::context>)
{
    return move();
}

accessor hal::access(std::string_view path)
{
    return path;
}

accessor hal::access(std::span<const std::byte> data)
{
    return data;
}

accessor hal::access(std::span<const std::uint8_t> data)
{
    return std::as_bytes(data);
}