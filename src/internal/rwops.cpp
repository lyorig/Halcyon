#include <halcyon/internal/rwops.hpp>

using namespace hal;

SDL_RWops* detail::rwops::get() const
{
    return ptr();
}

SDL_RWops* detail::rwops::use()
{
    return release();
}

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
    return rwops::get();
}

SDL_RWops* accessor::use(pass_key<surface>)
{
    return rwops::use();
}

SDL_RWops* accessor::use(pass_key<image::context>)
{
    return rwops::use();
}

SDL_RWops* accessor::use(pass_key<ttf::context>)
{
    return rwops::use();
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
    : rwops { ::SDL_RWFromFile(file.data(), "w") }
{
}

outputter::outputter(std::span<std::byte> data)
    : rwops { ::SDL_RWFromMem(data.data(), data.size()) }
{
}

outputter::outputter(std::span<std::uint8_t> data)
    : rwops { ::SDL_RWFromMem(data.data(), data.size()) }

{
}

SDL_RWops* outputter::use(pass_key<surface>)
{
    return rwops::use();
}

SDL_RWops* outputter::use(pass_key<image::context>)
{
    return rwops::use();
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