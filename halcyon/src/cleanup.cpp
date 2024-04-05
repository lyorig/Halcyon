#include <halcyon/cleanup.hpp>
#include <halcyon/debug.hpp>
#include <halcyon/internal/helpers.hpp>

using namespace hal;

cleanup::cleanup()
    : cleanup { {} }
{
}

cleanup::cleanup(std::initializer_list<system::type> subs)
{
    HAL_ASSERT_VITAL(::SDL_Init(il2bm<Uint32>(subs)) == 0, debug::last_error());
}

cleanup::~cleanup()
{
    HAL_PRINT("Cleaning up SDL. Last error: ", debug::last_error());
    ::SDL_Quit();
}