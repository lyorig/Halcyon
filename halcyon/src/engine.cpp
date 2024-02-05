#include <halcyon/engine.hpp>

#include <cstdlib>
#include <cstring>

#include <halcyon/debug.hpp>

using namespace hal;

engine::engine()
{
    HAL_ASSERT_VITAL(::SDL_Init(0) == 0, ::SDL_GetError());

    HAL_PRINT(debug::init, "Initialized SDL engine");
}

engine::~engine() { this->deinitialize(); }

void engine::exit() &
{
    this->deinitialize();
    std::exit(EXIT_SUCCESS);
}

void engine::deinitialize() const
{
    HAL_PRINT(debug::info, "Exiting. Last SDL error: ",
        lyo::is_c_string_empty(::SDL_GetError()) ? "none"
                                                 : ::SDL_GetError());
    ::SDL_Quit();
}