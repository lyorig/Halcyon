#include "image_loader.hpp"

#include "components/surface.hpp"

using namespace hal;

image_loader::image_loader(window& wnd, int type_flags) noexcept
{
    HAL_DEBUG_ASSERT(::IMG_Init(type_flags) == type_flags, ::IMG_GetError());

    HAL_DEBUG_PRINT(severity::init, "Initialized image loader with flags ", type_flags);
}

image_loader::~image_loader()
{
    ::IMG_Quit();
}

surface image_loader::load(const char* path) & noexcept
{
    HAL_DEBUG_PRINT(severity::info, "Loading image ", path);

    return ::IMG_Load(path);
}
