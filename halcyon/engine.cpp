#include "engine.hpp"

#include <cstring>

#include "debug.hpp"
#include "image_loader.hpp"
#include "internal/subsystem.hpp"
#include "mixer.hpp"
#include "window.hpp"

using namespace hal;

engine::engine() noexcept
{
    HAL_DEBUG_VERIFY(::SDL_Init(0) == 0, ::SDL_GetError());

    HAL_DEBUG_PRINT(debug::info, "Initialized engine");
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
    HAL_DEBUG_PRINT(debug::info, "Exiting. Last SDL error: ", std::strlen(::SDL_GetError()) > 0 ? ::SDL_GetError() : "none");
    ::SDL_Quit();
}