#include "chunk.hpp"

#include <halcyon/debug.hpp>

using namespace hal;

chunk::chunk(const char* path, lyo::pass_key<mixer>) noexcept :
    sdl_object { ::Mix_LoadWAV(path) }
{
}

void chunk::play() noexcept
{
    HAL_DEBUG_VERIFY(::Mix_PlayChannel(-1, m_object, 0) != -1, ::Mix_GetError());
}