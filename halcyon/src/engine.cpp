#include <halcyon/engine.hpp>

#include <cstdlib>
#include <cstring>

#include <halcyon/debug.hpp>

using namespace hal;

engine::engine()
{
    HAL_ASSERT_VITAL(::SDL_Init(SDL_INIT_VIDEO) == 0, ::SDL_GetError());

    HAL_PRINT(debug::init, "Initialized Halcyon, running on ", this->driver_name());
}

engine::~engine() { this->deinitialize(); }

void engine::exit() &
{
    this->deinitialize();
    std::exit(EXIT_SUCCESS);
}

std::string_view engine::driver_name()
{
    const char* ret { ::SDL_GetCurrentVideoDriver() };

    HAL_ASSERT(ret != nullptr, ::SDL_GetError());

    return ret;
}

void engine::deinitialize() const
{
    HAL_PRINT(debug::info, "Exiting. Last SDL error: ",
        lyo::is_c_string_empty(::SDL_GetError()) ? "none"
                                                 : ::SDL_GetError());
    ::SDL_Quit();
}