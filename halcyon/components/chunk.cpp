#include "chunk.hpp"

#include <halcyon/debug.hpp>

using namespace hal;

chunk::chunk(const char* path, lyo::pass_key<mixer>)
    : sdl_object { ::Mix_LoadWAV(path) }
{
    HAL_DEBUG_PRINT(severity::load, "Loaded SFX ", path);
}

void chunk::play(lyo::u16 loops) & { this->internal_play(loops); }

void chunk::play(infinite_loop_tag) & { this->internal_play(-1); }

void chunk::internal_play(int loops)
{
    HAL_DEBUG_ASSERT_VITAL(::Mix_PlayChannel(-1, m_object.get(), loops) != -1,
        ::Mix_GetError());
}