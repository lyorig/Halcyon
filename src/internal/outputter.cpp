#include <halcyon/internal/outputter.hpp>

#include <halcyon/debug.hpp>

using namespace hal;

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

SDL_RWops* outputter::get(pass_key<surface>)
{
    return m_ptr;
}

SDL_RWops* outputter::get(pass_key<image::context>)
{
    return m_ptr;
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