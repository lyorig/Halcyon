#pragma once

#include <halcyon/texture.hpp>

namespace quest
{
    class sprite
    {
    public:
        sprite(hal::renderer& rnd, const hal::surface& surf);

    private:
        hal::texture m_tex;

        hal::pixel_point m_dstSize;
    };

    class spritesheet : public sprite
    {
    public:
        using frame_t     = lyo::u8;
        using frame_point = hal::point<frame_t>;

        spritesheet(hal::renderer& rnd, const hal::surface& surf, hal::pixel_point frame_size);

    private:
        const frame_point m_size;
    };
}