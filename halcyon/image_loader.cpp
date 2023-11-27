#include "image_loader.hpp"

#include "components/surface.hpp"
#include "halcyon/internal/helpers.hpp"

using namespace hal;

image_loader::image_loader(engine& eng, il<image_type> types) noexcept
{
    HAL_DEBUG_ASSERT(::IMG_Init(il2bm<int>(types)) == il2bm<int>(types),
        ::IMG_GetError());

    HAL_DEBUG_PRINT(severity::init, "Initialized image loader with flags ",
        il2bm<int>(types));
}

image_loader::~image_loader() { ::IMG_Quit(); }

surface image_loader::load(const char* path) const noexcept
{
    HAL_DEBUG_PRINT(severity::info, "Loading image ", path);

    return ::IMG_Load(path);
}
