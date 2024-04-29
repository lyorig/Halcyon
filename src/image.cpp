#include <halcyon/image.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal::image;

context::context(std::initializer_list<load_format> types)
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

hal::surface context::load(accessor src) const
{
    return { ::IMG_Load_RW(src.use(pass_key<context> {}).get(), false), pass_key<context> {} };
}

hal::surface context::load(accessor src, load_format fmt) const
{
    return { ::IMG_LoadTyped_RW(src.use(pass_key<context> {}).get(), false, to_string(fmt).data()), pass_key<context> {} };
}

void context::save(const surface& surf, save_format fmt, outputter dst) const
{
    constexpr u8 jpg_quality { 90 };

    switch (fmt)
    {
        using enum save_format;
    case png:
        HAL_ASSERT_VITAL(::IMG_SavePNG_RW(surf.ptr(), dst.use(pass_key<context> {}).get(), false) == 0, debug::last_error());
        break;

    case jpg:
        HAL_ASSERT_VITAL(::IMG_SaveJPG_RW(surf.ptr(), dst.use(pass_key<context> {}).get(), false, jpg_quality) == 0, debug::last_error());
        break;
    }
}

query_format context::query(const accessor& src) const
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
        if (pair.first(src.get(pass_key<context> {})) != 0)
            return pair.second;

    return unknown;
}

bool context::initialized()
{
    return ::IMG_Init(0) > 0;
}

std::string_view hal::to_string(image::load_format fmt)
{
    using enum image::load_format;

    switch (fmt)
    {
    case jpg:
        return "JPG";

    case png:
        return "PNG";

    case tif:
        return "TIF";

    case webp:
        return "WEBP";

    case jxl:
        return "JXL";

    case avif:
        return "AVIF";
    }
}