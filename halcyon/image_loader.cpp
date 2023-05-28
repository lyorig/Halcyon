#include "image_loader.hpp"

#include "window.hpp"

using namespace halcyon;

image_loader::image_loader(engine& engine, int type_flags) noexcept
{
    HALCYON_VERIFY(::IMG_Init(type_flags) == type_flags, "SDL_image initialization failed");

    HALCYON_PRINT(debug::info, "Initialized image loader with flags ", type_flags);
}

image_loader::~image_loader()
{
    ::IMG_Quit();
}

surface image_loader::load_image(const char* path) const noexcept
{
    return path;
}
