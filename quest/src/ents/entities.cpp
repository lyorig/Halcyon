#include <quest/ents/entities.hpp>

using namespace quest::ent;

npc::npc(ID id, hal::texture&& t, const coord& spawnpoint)
    : non_unique { id }
    , scaleable { std::move(t), spawnpoint }
{
}