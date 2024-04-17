#include <halcyon/image.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal::image;

context::context(std::initializer_list<format> types)
{
    HAL_WARN_IF(initialized(), "Image context already exists");

    HAL_ASSERT_VITAL(::IMG_Init(detail::to_bitmask<int>(types)) == detail::to_bitmask<int>(types), ::IMG_GetError());

    HAL_PRINT(debug::severity::init, "Initialized image context, flags = 0x", std::hex, detail::to_bitmask<int>(types));
}

context::~context()
{
    HAL_ASSERT(initialized(), "Image context not initialized at destruction");

    ::IMG_Quit();

    HAL_PRINT("Destroyed image context");
}

hal::surface context::load(accessor data) const
{
    return { *this, std::move(data) };
}

bool context::initialized()
{
    return ::IMG_Init(0) > 0;
}