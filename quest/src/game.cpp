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
    while (event.poll())
    {
        switch (event.type())
        {
            using enum hal::event::event_type;

        case quit_requested:
            return false;

        case key_pressed:
            if (!event.keyboard().repeat())
                process_press(event.keyboard().button());
            break;

        case key_released:
            process_release(event.keyboard().button());
            break;

        default:
            break;
        }
    }

    return true;
}

void game::process_press(hal::keyboard::button b)
{
    using enum hal::keyboard::button;

    switch (b)
    {
    case Z:
        HAL_PRINT("Z pressed");
        break;
    case one:
        HAL_PRINT("1 pressed");
        break;
    default:
        break;
    }
}

void game::process_release(hal::keyboard::button b)
{
    using enum hal::keyboard::button;

    switch (b)
    {
    default:
        break;
    }
}