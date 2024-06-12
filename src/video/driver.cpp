#include <halcyon/video/driver.hpp>

#include <SDL_video.h>

#include <halcyon/debug.hpp>
#include <halcyon/video/renderer.hpp>

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

info::sdl::renderer driver::info(index_t idx)
{
    info::sdl::renderer ret;

    HAL_ASSERT_VITAL(::SDL_GetRenderDriverInfo(idx, ret.get()) == 0, debug::last_error());

    return ret;
}