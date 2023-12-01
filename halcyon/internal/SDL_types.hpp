#pragma once

#include <SDL2/SDL_rect.h>
#include <lyo/concepts.hpp>

namespace hal {
using SDL_pixel_type = decltype(SDL_Rect::x);
using SDL_position_type = decltype(SDL_FRect::x);

template <lyo::arithmetic T>
using SDL_rect_type = std::conditional_t<std::is_same_v<T, SDL_pixel_type>, SDL_Rect, SDL_FRect>;

template <lyo::arithmetic T>
using SDL_point_type = std::conditional_t<std::is_same_v<T, SDL_pixel_type>, SDL_Point, SDL_FPoint>;
}