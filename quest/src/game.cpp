#include <quest/constants.hpp>
#include <quest/game.hpp>

using namespace quest;

game::game(lyo::parser p)
{
    (void)p;

    m_renderer.size(constants::rpx_size(m_window.size()));
    HAL_PRINT("Set game size to ", m_renderer.size());

    m_cam.pos = m_renderer.size() / 2.0;

    m_ents.spawn<ent::npc>(
        hal::texture { m_renderer, hal::image_loader::load("assets/test_sprite.png") },
        coord { 0, 0 });
}

void game::main_loop()
{
    hal::ttf_engine teng;

    hal::event event;

    lyo::steady_timer timer;
    delta_t           delta;

    while (true)
    {
        while (event.poll())
        {
            switch (event.data.type)
            {
                using enum hal::event::type;

            case quit_requested:
                return;

            case key_pressed:
                if (event.data.key.repeat == 0 && !process_press(hal::button(event.data.key.keysym.scancode)))
                    return;
                break;

            case key_released:
                process_release(hal::button(event.data.key.keysym.scancode));
                break;

            default:
                break;
            }
        }

        delta = timer();
        timer.reset();

        // Update everything.
        m_ents.visit([&](auto& ent)
            { ent.update(delta * m_timescale); });

        // Draw everything.
        m_ents.visit([this](const auto& ent)
            { ent.draw(m_renderer, m_cam); });

        m_renderer.present();
    }
}

bool game::process_press(hal::button b)
{
    using enum hal::button;

    switch (b)
    {
    case esc:
        return false;

    default:
        break;
    }

    return true;
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