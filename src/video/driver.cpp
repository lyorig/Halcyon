#include <halcyon/video/driver.hpp>

#include <SDL3/SDL_video.h>

#include <halcyon/debug.hpp>

using namespace hal;

driver::index_t driver::amount()
{
    const auto ret = ::SDL_GetNumVideoDrivers();

    HAL_ASSERT(ret >= 1, debug::last_error());

    return static_cast<index_t>(ret);
}

std::string_view driver::name()
{
    const char* ret { ::SDL_GetCurrentVideoDriver() };

    HAL_ASSERT(ret != nullptr, debug::last_error());

    return ret;
}

std::string_view driver::name(index_t idx)
{
    const char* ret { ::SDL_GetVideoDriver(idx) };

    HAL_ASSERT(ret != nullptr, debug::last_error());

    return ret;
}