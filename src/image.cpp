#include <halcyon/image.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal::image;

context::context(std::initializer_list<init_format> types)
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
    return { ::IMG_Load_RW(src.use(pass_key<context> {}), true), pass_key<context> {} };
}

hal::surface context::load(accessor src, load_format fmt) const
{
    constexpr pass_key<context> pk;

    SDL_RWops* const ptr { src.get(pk) };

    switch (fmt)
    {
        using enum load_format;

    case jpg:
        return { ::IMG_LoadJPG_RW(ptr), pk };

    case png:
        return { ::IMG_LoadPNG_RW(ptr), pk };

    case tif:
        return { ::IMG_LoadTIF_RW(ptr), pk };

    case webp:
        return { ::IMG_LoadWEBP_RW(ptr), pk };

    case jxl:
        return { ::IMG_LoadJXL_RW(ptr), pk };

    case avif:
        return { ::IMG_LoadAVIF_RW(ptr), pk };

    case ico:
        return { ::IMG_LoadICO_RW(ptr), pk };

    case cur:
        return { ::IMG_LoadCUR_RW(ptr), pk };

    case bmp:
        return { ::IMG_LoadBMP_RW(ptr), pk };

    case gif:
        return { ::IMG_LoadGIF_RW(ptr), pk };

    case lbm:
        return { ::IMG_LoadLBM_RW(ptr), pk };

    case pcx:
        return { ::IMG_LoadPCX_RW(ptr), pk };

    case pnm:
        return { ::IMG_LoadPNM_RW(ptr), pk };

    case svg:
        return { ::IMG_LoadSVG_RW(ptr), pk };

    case qoi:
        return { ::IMG_LoadQOI_RW(ptr), pk };

    case xcf:
        return { ::IMG_LoadXCF_RW(ptr), pk };

    case xpm:
        return { ::IMG_LoadXPM_RW(ptr), pk };

    case xv:
        return { ::IMG_LoadXV_RW(ptr), pk };
        
    case unknown:
        HAL_PANIC("Trying to load an image of type \"unknown\"");
    };

    std::unreachable();
}

void context::save(const surface& surf, save_format fmt, outputter dst) const
{
    constexpr u8 jpg_quality { 90 };

    switch (fmt)
    {
        using enum save_format;
    case png:
        HAL_ASSERT_VITAL(::IMG_SavePNG_RW(const_cast<surface::pointer>(surf.get()), dst.use(pass_key<context> {}), true) == 0, debug::last_error());
        break;

    case jpg:
        HAL_ASSERT_VITAL(::IMG_SaveJPG_RW(const_cast<surface::pointer>(surf.get()), dst.use(pass_key<context> {}), true, jpg_quality) == 0, debug::last_error());
        break;
    }
}

load_format context::query(const accessor& src) const
{
    using enum load_format;

    constexpr std::pair<func_ptr<int, SDL_RWops*>, load_format> checks[] {
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

std::string_view hal::to_string(image::init_format fmt)
{
    using enum image::init_format;

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

    std::unreachable();
}