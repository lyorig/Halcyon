#include <quest/entities.hpp>
#include <utility>

using namespace quest;

player::player(ID id, hal::texture&& tex)
    : entity { id }
    , m_tex { std::move(tex) }
{
}

void player::update(lyo::f64 elapsed)
{
    constexpr lyo::f64 move_speed { 40.0 }, drag_speed { 30.0 }, drift_mul { 2.0 };

    if (m_moving)
        m_vel.x += move_speed * elapsed * m_dir;

    if (m_dir == dir::right)
    {
        if (m_vel.x < 0.0)
        {
            m_vel.x += drag_speed * elapsed * drift_mul;
        }

        else
            m_vel.x = std::max(m_vel.x - drag_speed * elapsed, 0.0);
    }

    else // left
    {
        if (m_vel.x > 0.0)
        {
            m_vel.x -= drag_speed * elapsed * drift_mul;
        }

        else
            m_vel.x = std::min(m_vel.x + drag_speed * elapsed, 0.0);
    }

    m_pos += m_vel;
}

void player::set_direction(dir d)
{
    m_dir = d;
}

void player::set_moving(bool val)
{
    m_moving = val;
}

void player::set_pos(pos p)
{
    m_pos = p;
}

void player::draw(hal::renderer& rnd) const
{
    rnd.draw(m_tex).to(m_pos).scale(apx_scale).flip(m_dir == dir::right ? hal::flip::none : hal::flip::x)();
}

void bullet_shell::update(lyo::f64 elapsed)
{
    (void)elapsed;
}

void bullet_shell::draw(hal::renderer& rnd) const
{
    (void)rnd;
}

void light::draw(hal::renderer& rnd) const
{
    (void)rnd;
}