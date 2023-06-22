#include "surface.hpp"

#include "font.hpp"

using namespace hal;

surface::surface(SDL_Surface* surf) noexcept :
    sdl_object { surf }
{
}

pixel_size surface::size() const noexcept
{
    return { static_cast<pixel_t>(m_object->w), static_cast<pixel_t>(m_object->h) };
}