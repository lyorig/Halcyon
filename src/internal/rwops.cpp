#include <halcyon/internal/rwops.hpp>

using namespace hal;

accessor::accessor(std::string_view path)
    : object { ::SDL_RWFromFile(path.data(), "r") }
{
}

accessor::accessor(std::span<const std::byte> data)
    : object { ::SDL_RWFromConstMem(data.data(), data.size_bytes()) }
{
}

SDL_RWops* accessor::get(pass_key<surface>) const
{
    return ptr();
}

SDL_RWops* accessor::get(pass_key<image::context>) const
{
    return ptr();
}

SDL_RWops* accessor::get(pass_key<ttf::context>) const
{
    return ptr();
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

outputter::outputter(std::string_view file)
    : object { ::SDL_RWFromFile(file.data(), "w") }
{
}

outputter::outputter(std::span<std::byte> data)
    : object { ::SDL_RWFromMem(data.data(), data.size()) }
{
}

outputter::outputter(std::span<std::uint8_t> data)
    : object { ::SDL_RWFromMem(data.data(), data.size()) }

{
}

SDL_RWops* outputter::get(pass_key<surface>) const
{
    return ptr();
}

SDL_RWops* outputter::get(pass_key<image::context>) const
{
    return ptr();
}

outputter hal::output(std::string_view file)
{
    return file;
}

outputter hal::output(std::span<std::byte> data)
{
    return data;
}

outputter hal::output(std::span<std::uint8_t> data)
{
    return data;
}