#include <SDL_video.h>

#include <halcyon/debug.hpp>
#include <halcyon/other/driver.hpp>

using namespace hal;

driver::index_t driver::amount()
{
    const auto ret = ::SDL_GetNumVideoDrivers();

    HAL_ASSERT(ret >= 1, ::SDL_GetError());

    return ret;
}

std::string_view driver::name()
{
    const char* ret { ::SDL_GetCurrentVideoDriver() };

    HAL_ASSERT(ret != nullptr, ::SDL_GetError());

    return ret;
}

std::string_view driver::name(index_t idx)
{
    const char* ret { ::SDL_GetVideoDriver(idx) };

    HAL_ASSERT(ret != nullptr, ::SDL_GetError());

    return ret;
}