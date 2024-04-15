#include <halcyon/debug.hpp>
#include <halcyon/internal/accessor.hpp>

using namespace hal;

SDL_RWops* accessor::get(lyo::pass_key<surface>)
{
    return m_ops;
}

SDL_RWops* accessor::get(lyo::pass_key<ttf::font>)
{
    return m_ops;
}

accessor::accessor(SDL_RWops* ptr)
    : m_ops { ptr }
{
    HAL_ASSERT(m_ops != nullptr, debug::last_error());
}

accessor hal::from_file(std::string_view file)
{
    return ::SDL_RWFromFile(file.data(), "r");
}

accessor hal::from_memory(std::span<const std::uint8_t> data)
{
    return ::SDL_RWFromConstMem(data.data(), data.size());
}