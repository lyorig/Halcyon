#include <quest/constants.hpp>
#include <quest/ents/components.hpp>

using namespace quest::ent::cmp;

scaleable::scaleable(hal::texture&& t, const coord& spawnpoint)
    : tex { std::move(t) }
    , pos { spawnpoint }
    , size(tex.size() * constants::apx_scale)
{
}

void scaleable::resize(scale_t scl)
{
    size = tex.size() * constants::apx_scale * scl;
}

void scaleable::draw(hal::renderer& rnd, const camera& cam) const
{
    rnd.draw(tex)
        .to(cam.transform({ pos, size }, rnd.size()))();
}