#include "engine.hpp"

#include <cstdlib>
#include <cstring>

#include "debug.hpp"

using namespace hal;

engine::engine()
{
    HAL_DEBUG_ASSERT(::SDL_Init(0) == 0, ::SDL_GetError());

    HAL_DEBUG_PRINT(severity::init, "Initialized SDL engine");
}

engine::~engine() { this->deinitialize(); }

void engine::exit() &
{
    this->deinitialize();
    std::exit(EXIT_SUCCESS);
}

void engine::deinitialize() const
{
    HAL_DEBUG_PRINT(severity::info, "Exiting. Last SDL error: ",
        lyo::is_c_string_empty(::SDL_GetError()) ? "none"
                                                 : ::SDL_GetError());
    ::SDL_Quit();
}