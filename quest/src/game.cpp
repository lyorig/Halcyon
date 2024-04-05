#include <quest/constants.hpp>
#include <quest/game.hpp>

using namespace quest;

game::game()
{
    // Set a unified renderer size.
    m_renderer.size(constants::rpx_size(m_window.size()));
    HAL_PRINT("Set game size to ", m_renderer.size());

    m_cam.pos.jump(m_renderer.size() / 2.0);

    m_ents.spawn<ent::npc>(hal::texture { m_renderer, hal::image::load(hal::from_file("assets/test_sprite.png")) }, coord { 100, 100 });
}

void game::main_loop()
{
    hal::event event;

    lyo::steady_timer timer;
    delta_t           delta;

    while (true)
    {
        if (!process_events(event))
            return;

        delta = timer();
        timer.reset();

        // Update everything.
        m_ents.visit([&](auto& obj)
            { obj.update(delta * m_timescale.value()); });

        // Timescale updates obviously aren't affected by itself.
        m_timescale.update(delta);
        m_cam.update(delta * m_timescale.value());

        // Render everything.
        m_ents.visit([this](const auto& obj)
            { obj.draw(m_renderer, m_cam); });

        m_renderer.present();
    }
}

bool game::process_events(hal::event& event)
{
    constexpr hitbox area { 100, 100, 400, 400 };
    hal::color_lock  cl { m_renderer, hal::color::weezer_blue };
    m_renderer.fill_rect(m_cam.transform(area, m_renderer.size()));

    auto& first = m_ents.view<ent::npc>().front();

    while (event.poll())
    {
        switch (event.data.type)
        {
            using enum hal::event::system_event;

        case quit_requested:
            return false;

        case key_pressed:
            if (event.data.key.repeat == 0)
                first.pos = area.anchor(process_press(hal::button(event.data.key.keysym.scancode)), first.size);
            break;

        case key_released:
            process_release(hal::button(event.data.key.keysym.scancode));
            break;

        default:
            break;
        }
    }

    return true;
}

hal::anchor game::process_press(hal::button b)
{
    using enum hal::button;
    using enum hal::anchor;

    switch (b)
    {
    case one:
        return top_left;

    case two:
        return top_right;

    case three:
        return bottom_left;

    case four:
        return bottom_right;

    default:
        return center;
    }
}

void game::process_release(hal::button b)
{
    using enum hal::button;

    switch (b)
    {
    default:
        break;
    }
}