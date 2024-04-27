#include <halcyon/internal/sdl_rwops.hpp>

#include <halcyon/debug.hpp>

using namespace hal::sdl;

rwops::rwops(SDL_RWops* ptr)
    : m_ptr { ptr }
{
    HAL_ASSERT(ptr != nullptr, debug::last_error());
}