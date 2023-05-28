#include "chunk.hpp"

#include <halcyon/debug.hpp>

using namespace halcyon;

chunk::chunk(const char* path) noexcept :
    sdl_object { ::Mix_LoadWAV(path), "Couldn't load chunk" }
{
}

void chunk::play() noexcept
{
    HALCYON_VERIFY(::Mix_PlayChannel(-1, m_object, 0) != -1, "Couldn't play chunk");
}