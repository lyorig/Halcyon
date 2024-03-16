#include <quest/ents/components.hpp>

using namespace quest::ent::cmp;

drawable::drawable(hal::texture&& t)
    : tex { std::move(t) }
{
}

scaleable::scaleable(hal::texture&& t)
    : drawable { std::move(t) }
    , size(tex.size())
{
}

void scaleable::resize(scale_t scl)
{
    size = tex.size() * scl;
}

void scaleable::draw(hal::renderer& rnd, const camera& cam) const
{
    rnd.draw(tex).to(cam.transform({ pos, size }, rnd.size().y))();
}