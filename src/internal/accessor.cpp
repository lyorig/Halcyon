#include <halcyon/debug.hpp>
#include <halcyon/internal/accessor.hpp>

using namespace hal;

accessor::accessor(std::string_view path)
    : rwops { ::SDL_RWFromFile(path.data(), "r") }
{
}

accessor::accessor(std::span<const std::byte> data)
    : rwops { ::SDL_RWFromConstMem(data.data(), data.size_bytes()) }
{
}

SDL_RWops* accessor::get(pass_key<image::context>) const
{
    return m_ptr;
}

SDL_RWops* accessor::get(pass_key<ttf::context>) const
{
    return m_ptr;
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