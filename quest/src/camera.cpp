#include <quest/camera.hpp>

using namespace quest;

// The camera's velocity when going to a different target, in m/s.
constexpr coord CamVel { 512, 512 };

camera::camera(const hal::renderer& rnd)
    : m_pos { rnd.size() / 2.0f }
{
}

void camera::update(delta_t elapsed)
{
    // No target; nothing to do.
    if (!m_tgtPos)
        return;

    // Travelling complete; copy position and return.
    if (m_travelTime <= 0.0)
    {
        m_pos = *m_tgtPos;
        return;
    }

    const auto dist = *m_tgtPos - m_pos;

    const direction neg[2] {
        dist.x >= 0 ? right : left,
        dist.y >= 0 ? up : down
    };

    m_pos.x += std::min<meter_t>(CamVel.x * elapsed, std::abs(dist.x)) * std::to_underlying(neg[0]);
    m_pos.y += std::min<meter_t>(CamVel.y * elapsed, std::abs(dist.y)) * std::to_underlying(neg[1]);
}

void camera::move(const coord& pos)
{
    m_tgtPos = nullptr;
    m_pos    = pos;
}

void camera::target(const coord& pos)
{
    m_tgtPos = &pos;
}

hitbox camera::transform(hitbox hbx, hal::pixel_point rsz) const
{
    hbx.pos.x += rsz.x / 2.0 - m_pos.x;
    hbx.pos.y = rsz.y - hbx.pos.y - hbx.size.y - (rsz.y / 2.0 - m_pos.y);

    return hbx;
}