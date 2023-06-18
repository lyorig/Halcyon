#include "image_loader.hpp"

using namespace hal;

image_loader::image_loader(window& wnd, int type_flags) noexcept
{
    HAL_DEBUG_VERIFY(::IMG_Init(type_flags) == type_flags, ::SDL_GetError());

    HAL_DEBUG_PRINT(success, "Initialized image loader with flags ", type_flags);
}

image_loader::~image_loader()
{
    ::IMG_Quit();
}

surface image_loader::load_image(const char* path) const noexcept
{
    return { path, {} };
}
