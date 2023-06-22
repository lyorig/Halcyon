#include "image_loader.hpp"

using namespace hal;

image_loader::image_loader(window& wnd, int type_flags) noexcept
{
    HAL_ASSERT(::IMG_Init(type_flags) == type_flags, ::IMG_GetError());

    HAL_PRINT(severity::success, "Initialized image loader with flags ", type_flags);
}

image_loader::~image_loader()
{
    ::IMG_Quit();
}

surface image_loader::load(const char* path) const noexcept
{
    SDL_Surface* surf { ::IMG_Load(path) };

    HAL_CHECK(surf != nullptr, ::IMG_GetError());

    return surf;
}
