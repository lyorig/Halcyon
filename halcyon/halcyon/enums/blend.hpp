#pragma once

#include <SDL2/SDL_blendmode.h>
#include <lyoSTL/types.hpp>

namespace hal
{
    enum class blend_mode : lyo::u8
    {
        none  = SDL_BLENDMODE_NONE,
        add   = SDL_BLENDMODE_ADD,
        mod   = SDL_BLENDMODE_MOD,
        mul   = SDL_BLENDMODE_MUL,
        blend = SDL_BLENDMODE_BLEND
    };
}