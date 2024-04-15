#include <halcyon/image.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal;

image::context::context(std::initializer_list<format> types)
{
    HAL_ASSERT_VITAL(::IMG_Init(detail::to_bitmask<int>(types)) == detail::to_bitmask<int>(types), ::IMG_GetError());

    HAL_PRINT(hal::severity::init, "Initialized image loader, flags = 0x", std::hex, detail::to_bitmask<int>(types));
}

image::context::~context() { ::IMG_Quit(); }

bool image::context::initialized()
{
    return ::IMG_Init(0) > 0;
}

surface image::context::load(accessor data)
{
    return ::IMG_Load_RW(data.get(), false);
}