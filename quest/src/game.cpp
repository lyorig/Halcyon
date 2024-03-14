#include <quest/constants.hpp>
#include <quest/game.hpp>

using namespace quest;

game::game(lyo::parser p)
{
    (void)p;

    m_renderer.size(constants::rpx_size(m_window.size()));
    HAL_PRINT("Set game size to ", m_renderer.size());

    hal::texture tex { m_renderer, hal::image_loader::load("assets/test_sprite.png") };
}

void game::main_loop()
{
    hal::ttf_engine teng;

    hal::event event;

    lyo::steady_timer timer;
    delta_t           delta;

    hal::texture vt;
    hal::font    f { "assets/m5x7.ttf", 42 };

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
                this->process_press(hal::button(event.data.key.keysym.scancode));
                break;

            case key_released:
                this->process_release(hal::button(event.data.key.keysym.scancode));
                break;

            default:
                break;
            }
        }

        delta = timer();
        timer.reset();

        // Update everything.
        m_ents.visit([&]<updateable T>(T& ent)
            { ent.update(delta * m_timescale); });

        m_sents.visit([&](auto& ent)
            { ent.update(delta * m_timescale); });

        m_renderer.draw(vt)
            .to({ 0, 50 })();

        // Draw everything.
        m_ents.visit([this]<drawable T>(const T& ent)
            { ent.draw(m_renderer, m_cam); });

        m_renderer.present();
    }
}

void game::process_press(hal::button b)
{
    using enum hal::button;
    using namespace std::chrono_literals;

    switch (b)
    {
    case A:
        break;

    case D:
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
    case D:
        break;
    default:
        break;
    }
}