#include <quest/constants.hpp>
#include <quest/game.hpp>
#include <quest/timeout.hpp>

using namespace quest;

game::game(lyo::parser p)
{
    (void)p;

    m_renderer.size(constants::rpx_size(m_window.size()));
    HAL_PRINT("Set game size to ", m_renderer.size());

    m_cam.pos.jump(m_renderer.size() / 2.0);

    m_ents.spawn<ent::npc>(hal::texture { m_renderer, hal::image_loader::load("assets/test_sprite.png") }, coord { 100, 100 });
}

void game::main_loop()
{
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

        // Do all game updates here.
        m_ents.visit([&](auto& obj)
            { obj.update(delta * m_timescale.value()); });

        // Timescale updates obviously aren't affected by itself.
        m_timescale.update(delta);
        m_cam.update(delta * m_timescale.value());

        // Do all rendering here.
        m_ents.visit([this](const auto& obj)
            { obj.draw(m_renderer, m_cam); });

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
    case one:
        m_cam.pos.start({ 1000, 100 }, 1.0);
        break;
    case two:
        m_cam.pos.start({ 0, 100 }, 1.0);
        break;
    case three:
        m_timescale.start(0.1, 1.0);
        break;
    case four:
        m_timescale.start(1.0, 1.0);
        break;
    case five:
        set_timeout([]()
            { std::cout << "Sus"; },
            std::chrono::seconds { 1 });
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

bool game::held(hal::button b)
{
    return hal::event::keyboard_state()[std::to_underlying(b)];
}