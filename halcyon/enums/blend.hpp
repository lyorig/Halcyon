#pragma once

#include <SDL2/SDL_blendmode.h>

namespace hal
{
    enum class blend_mode : std::underlying_type_t<SDL_BlendMode>
    {
        none = SDL_BLENDMODE_NONE,
        add = SDL_BLENDMODE_ADD,
        mod = SDL_BLENDMODE_MOD,
        mul = SDL_BLENDMODE_MUL,
        blend = SDL_BLENDMODE_BLEND
    };
}