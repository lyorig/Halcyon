#include <halcyon/image_loader.hpp>

#include <halcyon/other/helpers.hpp>
#include <halcyon/surface.hpp>

using namespace hal;

image_loader::image_loader(std::initializer_list<format> types)
{
    HAL_ASSERT_VITAL(::IMG_Init(il2bm<int>(types)) == il2bm<int>(types), ::IMG_GetError());

    HAL_PRINT(hal::debug::init, "Initialized image loader, flags = 0x", std::hex, il2bm<int>(types));
}

image_loader::~image_loader() { ::IMG_Quit(); }

bool image_loader::initialized()
{
    return ::IMG_Init(0) > 0;
}

surface hal::load_image(accessor data)
{
    return ::IMG_Load_RW(data.get(), false);
}