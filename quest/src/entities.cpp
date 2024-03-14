#include <quest/constants.hpp>
#include <quest/entities.hpp>

using namespace quest;

non_unique_entity::non_unique_entity(ID id)
    : m_id { id }
{
}

non_unique_entity::ID non_unique_entity::id() const
{
    return m_id;
}

npc::npc(ID id, hal::texture&& tex, coord spawn_point)
    : non_unique_entity { id }
    , pos { spawn_point }
    , tex { std::move(tex) }
{
}

void npc::update(delta_t elapsed)
{
    constexpr meter_t Acceleration { 40.0 }, MaxSpeed { 5.0 }, Friction { 20.0 }, DriftMul { 2.0 };

    // If moving, increase velocity.
    if (moving)
    {
        vel.x += Acceleration * elapsed * std::to_underlying(dir);
    }

    // Apply velocity.
    pos += vel;

    if (vel.x * std::to_underlying(dir) < 0.0)
        vel.x = std::min(-std::abs(vel.x) + Friction * elapsed * DriftMul, 0.0) * std::to_underlying(dir);
    else // Apply friction.
        vel.x = std::clamp<meter_t>(std::abs(vel.x) - Friction * elapsed, 0.0, MaxSpeed) * std::to_underlying(dir);
}

void npc::draw(hal::renderer& rnd, const camera& cam) const
{
    rnd.draw(tex)
        .to(cam.transform(pos, hitbox().y, rnd.size().y))
        .scale(constants::apx_scale)
        .flip(dir == right ? hal::flip::none : hal::flip::x)();
}

coord npc::hitbox() const
{
    return tex.size() * constants::apx_scale * scale;
}
