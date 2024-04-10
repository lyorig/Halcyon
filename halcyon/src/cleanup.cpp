#include <halcyon/cleanup.hpp>
#include <halcyon/debug.hpp>
#include <halcyon/internal/helpers.hpp>

using namespace hal;

cleanup::cleanup()
    : cleanup { {} }
{
}

cleanup::cleanup(std::initializer_list<system> subs)
{
    HAL_ASSERT_VITAL(::SDL_Init(detail::to_bitmask<std::uint32_t>(subs)) == 0, debug::last_error());
    HAL_PRINT("Initialized SDL, flags ", std::hex, "0x", detail::to_bitmask<std::uint32_t>(subs));
}

cleanup::~cleanup()
{
    HAL_PRINT("Cleaning up SDL. Last error: ", debug::last_error());
    ::SDL_Quit();
}