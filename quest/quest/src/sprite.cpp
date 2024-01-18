#include <quest/sprite.hpp>

using namespace quest;

sprite::sprite(hal::renderer& rnd, const hal::surface& surf)
    : m_tex { rnd, surf }
{
}

spritesheet::spritesheet(hal::renderer& rnd, const hal::surface& surf, hal::pixel_point frame_size)
    : sprite { rnd, surf }
    , m_size { frame_point(surf.size() / frame_size) }
{
}