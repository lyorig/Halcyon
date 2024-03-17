#pragma once

#include <quest/ents/components.hpp>
#include <quest/ents/modifiers.hpp>

namespace quest::ent
{
    struct npc : mod::non_unique, cmp::scaleable
    {
        npc(ID id, hal::texture&& t, const coord& spawnpoint);
    };

    struct player : mod::unique, cmp::scaleable
    {
    };
}