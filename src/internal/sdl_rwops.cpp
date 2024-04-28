#include <halcyon/internal/sdl_rwops.hpp>

#include <halcyon/debug.hpp>

using namespace hal::sdl;

rwops::rwops(SDL_RWops* ptr)
    : m_ptr { ptr }
{
    HAL_ASSERT(ptr != nullptr, debug::last_error());
}

void rwops::free() const
{
    HAL_WARN_IF_VITAL(::SDL_RWclose(m_ptr) == -1, "RWops failed to flush to disk; ", debug::last_error());
}