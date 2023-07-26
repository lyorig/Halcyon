#include "surface.hpp"

using namespace hal;

surface::surface(SDL_Surface* surf) noexcept :
    sdl_object { surf }
{
}

pixel_size surface::size() const noexcept
{
    return { static_cast<pixel_type>(m_object->w), static_cast<pixel_type>(m_object->h) };
}