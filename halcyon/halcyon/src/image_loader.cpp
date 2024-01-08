#include <halcyon/image_loader.hpp>

#include <halcyon/components/surface.hpp>
#include <halcyon/internal/helpers.hpp>

using namespace hal;

image_loader::image_loader(video& sys [[maybe_unused]], std::initializer_list<image_type> types)
{
    HAL_DEBUG_ASSERT_VITAL(::IMG_Init(il2bm<int>(types)) == il2bm<int>(types),
        ::IMG_GetError());

    HAL_DEBUG_PRINT(hal::debug::init, "Initialized image loader with flags ", il2bm<int>(types));
}

image_loader::~image_loader() { ::IMG_Quit(); }

surface image_loader::load(const char* path) const
{
    HAL_DEBUG_PRINT("Loading image ", path);

    return ::IMG_Load(path);
}
