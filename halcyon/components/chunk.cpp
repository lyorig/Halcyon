#include "chunk.hpp"

#include <halcyon/debug.hpp>

using namespace hal;

chunk::chunk(const char* path, lyo::pass_key<mixer>) noexcept :
    sdl_object { ::Mix_LoadWAV(path) }
{
    HAL_DEBUG_PRINT(severity::load, "Loaded SFX ", path);
}

void chunk::play(loop_type loops) noexcept
{
    HAL_DEBUG_ASSERT(::Mix_PlayChannel(-1, m_object.get(), loops) != -1, ::Mix_GetError());
}