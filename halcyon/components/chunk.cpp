#include "chunk.hpp"

#include <halcyon/debug.hpp>

using namespace hal;

chunk::chunk(const char* path, lyo::pass_key<mixer>)
    : sdl_object { ::Mix_LoadWAV(path) }
{
    HAL_DEBUG_PRINT(debug::load, "Loaded SFX ", path);
}

void chunk::play(lyo::u16 loops) const
{
    this->internal_play(loops);
}

void chunk::play(infinite_loop_tag) const
{
    this->internal_play(-1);
}

void chunk::internal_play(int loops) const
{
    HAL_DEBUG_ASSERT_VITAL(::Mix_PlayChannel(-1, this->ptr(), loops) != -1,
        ::Mix_GetError());
}