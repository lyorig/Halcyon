#include <halcyon/debug.hpp>
#include <halcyon/internal/accessor.hpp>

using namespace hal;

accessor::accessor(std::string_view path)
    : accessor { ::SDL_IOFromFile(path.data(), "r") }
{
}

accessor::accessor(std::span<const std::byte> data)
    : accessor { ::SDL_IOFromConstMem(data.data(), data.size_bytes()) }
{
}

SDL_IOStream* accessor::get(pass_key<image::context>) const
{
    return m_ops;
}

SDL_IOStream* accessor::get(pass_key<ttf::context>) const
{
    return m_ops;
}

accessor::accessor(SDL_IOStream* ptr)
    : m_ops { ptr }
{
    HAL_ASSERT(m_ops != nullptr, debug::last_error());
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