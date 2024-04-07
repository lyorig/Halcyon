#include <halcyon/debug.hpp>
#include <halcyon/internal/system.hpp>

using namespace hal;

void system::initialize(system::type s)
{
    HAL_ASSERT(!initialized(s), "Initializing already initialized subsystem ", std::to_underlying(s));
    HAL_ASSERT_VITAL(::SDL_InitSubSystem(Uint32(s)) == 0, debug::last_error());
}

void system::deinitialize(system::type s)
{
    HAL_ASSERT(initialized(s), "Quitting uninitialized subsystem ", std::to_underlying(s));
    ::SDL_QuitSubSystem(Uint32(s));
}

bool system::initialized(system::type s)
{
    return ::SDL_WasInit(Uint32(s)) == Uint32(s);
}