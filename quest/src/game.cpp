#include <quest/game.hpp>

using namespace quest;

game::game(lyo::parser p)
    : m_cleanup { hal::system::video }
    , m_image { hal::image_loader::png }
    , m_window { "HalQuest", hal::fullscreen_mode }
    , m_renderer { m_window, { hal::renderer::accelerated, hal::renderer::vsync } }
    , m_ents { m_renderer }
{
    (void)p;
}

void game::main_loop()
{
    hal::event event;

    lyo::steady_timer timer;
    lyo::f64          delta;

    m_ents.player.set_pos({ 0, m_window.size().y * 0.75 });

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
                process_press(hal::button(event.data.key.keysym.scancode));
                break;

            case key_released:
                process_release(hal::button(event.data.key.keysym.scancode));
                break;

            default:
                break;
            }
        }

        delta = timer();

        // Update everything.
        m_ents.visit([&]<updateable T>(T& ent)
            { ent.update(delta); });

        timer.reset();

        // Draw everything.
        m_ents.visit([this]<drawable T>(const T& ent)
            { ent.draw(m_renderer); });

        m_renderer.present();
    }
}

void game::process_press(hal::button b)
{
    using enum hal::button;

    switch (b)
    {
    case A:
        m_ents.player.set_direction(dir::left);
        m_ents.player.set_moving(true);
        break;
    case D:
        m_ents.player.set_direction(dir::right);
        m_ents.player.set_moving(true);
        break;

    default:
        break;
    }
}

void game::process_release(hal::button b)
{
    using enum hal::button;

    switch (b)
    {
    case A:
        m_ents.player.set_moving(false);
        break;
    case D:
        m_ents.player.set_moving(false);
        break;
    default:
        return;
    }
}