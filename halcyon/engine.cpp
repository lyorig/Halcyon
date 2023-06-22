#include "engine.hpp"

#include <cstring>

#include "debug.hpp"

using namespace hal;

engine::engine() noexcept
{
    HAL_ASSERT(::SDL_Init(0) == 0, ::SDL_GetError());

    HAL_PRINT(severity::success, "Initialized engine");
}

engine::~engine()
{
    this->deinitialize();
}

void engine::exit() noexcept
{
    this->deinitialize();
    std::exit(EXIT_SUCCESS);
}

void engine::deinitialize() noexcept
{
    HAL_PRINT(severity::info, "Exiting. Last SDL error: ", std::strlen(::SDL_GetError()) > 0 ? ::SDL_GetError() : "none");
    ::SDL_Quit();
}