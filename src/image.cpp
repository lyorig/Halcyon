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
    return { ::IMG_Load_RW(data.get(pass_key<context> {}), true), pass_key<context> {} };
}

query_format context::query(const accessor& data) const
{
    using enum query_format;
    constexpr std::pair<func_ptr<int, SDL_RWops*>, query_format> checks[] {
        { ::IMG_isAVIF, avif },
        { ::IMG_isICO, ico },
        { ::IMG_isCUR, cur },
        { ::IMG_isBMP, bmp },
        { ::IMG_isGIF, gif },
        { ::IMG_isJPG, jpg },
        { ::IMG_isJXL, jxl },
        { ::IMG_isLBM, lbm },
        { ::IMG_isPCX, pcx },
        { ::IMG_isPNG, png },
        { ::IMG_isPNM, pnm },
        { ::IMG_isSVG, svg },
        { ::IMG_isQOI, qoi },
        { ::IMG_isTIF, tif },
        { ::IMG_isXCF, xcf },
        { ::IMG_isXPM, xpm },
        { ::IMG_isXV, xv },
        { ::IMG_isWEBP, webp }
    };

    for (const auto& pair : checks)
        if (pair.first(data.get(pass_key<context> {})) != 0)
            return pair.second;

    return unknown;
}

bool context::initialized()
{
    return ::IMG_Init(0) > 0;
}