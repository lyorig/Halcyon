#include <halcyon/internal/outputter.hpp>

#include <halcyon/debug.hpp>

using namespace hal;

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

SDL_RWops* outputter::get(pass_key<surface>)
{
    return ptr();
}

SDL_RWops* outputter::get(pass_key<image::context>)
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