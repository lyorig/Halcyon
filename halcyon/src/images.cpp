#include <halcyon/images.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal;

image::cleanup::cleanup(std::initializer_list<format> types)
{
    HAL_ASSERT_VITAL(::IMG_Init(il2bm<int>(types)) == il2bm<int>(types), ::IMG_GetError());

    HAL_PRINT(hal::debug::init, "Initialized image loader, flags = 0x", std::hex, il2bm<int>(types));
}

image::cleanup::~cleanup() { ::IMG_Quit(); }

bool image::cleanup::initialized()
{
    return ::IMG_Init(0) > 0;
}

surface image::load(accessor data)
{
    return ::IMG_Load_RW(data.get(), false);
}