#include "engine.hpp"

#include <cstring>

#include "debug.hpp"
#include "image_loader.hpp"
#include "internal/subsystem.hpp"
#include "mixer.hpp"
#include "window.hpp"

using namespace halcyon;

engine::engine() noexcept
{
    HALCYON_VERIFY(::SDL_Init(0) == 0, "SDL initialization failed");

    HALCYON_PRINT(debug::info, "Initialized engine");
}

engine::~engine()
{
    this->deinitialize();
}

display_info engine::monitor_info(lyo::u8 index) const noexcept
{
    subsystem<video> subsys;

    SDL_DisplayMode dm;

    HALCYON_VERIFY(::SDL_GetDesktopDisplayMode(index, &dm) == 0, "Couldn't get display info");

    return dm;
}

void engine::exit() noexcept
{
    this->deinitialize();
    std::exit(EXIT_SUCCESS);
}

void engine::deinitialize() noexcept
{
    HALCYON_PRINT(debug::info, "Exiting. Last SDL error: ", std::strlen(::SDL_GetError()) > 0 ? ::SDL_GetError() : "none");
    ::SDL_Quit();
}