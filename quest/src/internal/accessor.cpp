#include <halcyon/debug.hpp>
#include <halcyon/internal/accessor.hpp>

using namespace hal;

accessor::accessor(std::span<const std::byte> data)
    : accessor { ::SDL_RWFromConstMem(data.data(), data.size()) }
{
}

accessor::accessor(std::string_view file)
    : accessor { file.data() }
{
}

accessor::accessor(const char* file)
    : accessor { ::SDL_RWFromFile(file, "r") }
{
}

accessor::accessor(SDL_RWops* ptr)
    : m_ops { ptr }
{
    HAL_ASSERT(m_ops != nullptr, debug::last_error());
}

accessor::~accessor()
{
    ::SDL_RWclose(m_ops);
}

SDL_RWops* accessor::get()
{
    return m_ops;
}