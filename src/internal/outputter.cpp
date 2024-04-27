#include <halcyon/internal/outputter.hpp>

#include <halcyon/debug.hpp>

using namespace hal;

outputter::outputter(std::string_view file)
    : outputter { ::SDL_RWFromFile(file.data(), "w") }
{
}

outputter::outputter(std::span<std::byte> data)
    : outputter { ::SDL_RWFromMem(data.data(), data.size()) }
{
}

outputter::outputter(std::span<std::uint8_t> data)
    : outputter { ::SDL_RWFromMem(data.data(), data.size()) }

{
}

SDL_RWops* outputter::get(pass_key<surface>)
{
    return m_ops;
}

outputter::outputter(SDL_RWops* ptr)
    : m_ops { ptr }
{
    HAL_ASSERT(m_ops != nullptr, debug::last_error());
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