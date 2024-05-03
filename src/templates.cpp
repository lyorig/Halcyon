#include <SDL_render.h>
#include <SDL_surface.h>

#include <halcyon/types/rectangle.hpp>

#include <halcyon/internal/sdl_object.hpp>

namespace hal
{
    // Points.
    template struct point<std::int8_t>;
    template struct point<std::int16_t>;
    template struct point<std::int32_t>;
    template struct point<std::int64_t>;

    template struct point<std::uint8_t>;
    template struct point<std::uint16_t>;
    template struct point<std::uint32_t>;
    template struct point<std::uint64_t>;

    template struct point<float>;
    template struct point<double>;

    // Rectangles.
    template struct rectangle<std::int8_t>;
    template struct rectangle<std::int16_t>;
    template struct rectangle<std::int32_t>;
    template struct rectangle<std::int64_t>;

    template struct rectangle<std::uint8_t>;
    template struct rectangle<std::uint16_t>;
    template struct rectangle<std::uint32_t>;
    template struct rectangle<std::uint64_t>;

    template struct rectangle<float>;
    template struct rectangle<double>;

    // SDL RAII objects.
    template class detail::raii_object<SDL_Window, ::SDL_DestroyWindow>;
    template class detail::raii_object<SDL_Renderer, ::SDL_DestroyRenderer>;

    template class detail::raii_object<SDL_Surface, ::SDL_FreeSurface>;
    template class detail::raii_object<SDL_Texture, ::SDL_DestroyTexture>;

    template class detail::raii_object<SDL_RWops, ::SDL_RWclose>;
}