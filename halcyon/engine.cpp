#include "engine.hpp"

#include <cstring>

#include "debug.hpp"

using namespace hal;

engine::engine() noexcept
{
    HAL_DEBUG_ASSERT(::SDL_Init(0) == 0, ::SDL_GetError());

    HAL_DEBUG_PRINT(severity::init, "Initialized engine");
}

engine::~engine()
{
    this->deinitialize();
}

void engine::exit() & noexcept
{
    this->deinitialize();
    std::exit(EXIT_SUCCESS);
}

void engine::deinitialize() const noexcept
{
    HAL_DEBUG_PRINT(severity::info, "Exiting. Last SDL error: ", lyo::is_c_string_empty(::SDL_GetError()) ? "none" : ::SDL_GetError());
    ::SDL_Quit();
}