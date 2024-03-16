#include <halcyon/image_loader.hpp>

#include <halcyon/surface.hpp>
#include <halcyon/other/helpers.hpp>

using namespace hal;

image_loader::image_loader(std::initializer_list<format> types)
{
    HAL_ASSERT_VITAL(::IMG_Init(il2bm<int>(types)) == il2bm<int>(types), ::IMG_GetError());

    HAL_PRINT(hal::debug::init, "Initialized image loader, flags = 0x", std::hex, il2bm<int>(types));
}

image_loader::~image_loader() { ::IMG_Quit(); }

surface image_loader::load(std::string_view path)
{
    HAL_PRINT("Loading image ", path);

    return { ::IMG_Load(path.data()), lyo::pass_key<image_loader> {} };
}

bool image_loader::initialized()
{
    return ::IMG_Init(0) > 0;
}