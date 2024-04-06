#include <halcyon/debug.hpp>
#include <halcyon/internal/accessor.hpp>

using namespace hal;

detail::accessor::accessor(SDL_RWops* ptr)
    : m_ops { ptr }
{
    HAL_ASSERT(m_ops != nullptr, debug::last_error());
}

detail::accessor::~accessor()
{
    HAL_WARN_IF_VITAL(::SDL_RWclose(m_ops) != 0, "File failed to flush its output to disk: ", debug::last_error());
}

SDL_RWops* detail::accessor::get()
{
    return m_ops;
}

detail::accessor hal::from_file(std::string_view file)
{
    return ::SDL_RWFromFile(file.data(), "r");
}

detail::accessor hal::from_memory(std::span<const std::uint8_t> data)
{
    return ::SDL_RWFromConstMem(data.data(), data.size());
}