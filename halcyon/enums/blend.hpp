#pragma once

#include <SDL_blendmode.h>
#include <lyo/types.hpp>

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