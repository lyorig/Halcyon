#include <quest/game.hpp>

int main()
{
    constexpr quest::hitbox hbx { 100, 100, 400, 400 };
    quest::coord            current;

    hal::cleanup  c { hal::system::video };
    hal::window   wnd { "HalQuest Testing", hal::fullscreen };
    hal::renderer rnd { wnd, { hal::renderer::vsync, hal::renderer::accelerated } };

    hal::texture tex { rnd, hal::image::load(hal::from_file("assets/test_sprite.png")) };

    hal::event e;

    while (true)
    {
        while (e.poll())
        {
            using enum hal::event::event_type;
            using enum hal::keyboard::button;
            using enum hal::anchor;

            switch (e.type())
            {
            case quit_requested:
                return EXIT_SUCCESS;

            case key_pressed:
                switch (e.keyboard().button())
                {
                case one:
                    current = hbx.anchor(top_left, quest::coord(tex.size()));
                    break;

                case two:
                    current = hbx.anchor(top_right, quest::coord(tex.size()));
                    break;

                case three:
                    current = hbx.anchor(bottom_left, quest::coord(tex.size()));
                    break;

                case four:
                    current = hbx.anchor(bottom_right, quest::coord(tex.size()));
                    break;

                case five:
                    current = hbx.anchor(center, quest::coord(tex.size()));
                    break;

                default:
                    break;
                }

                break;

            default:
                break;
            }
        }

        {
            hal::color_lock cl { rnd, hal::color::weezer_blue };
            rnd.fill_rect(hbx);
        }

        rnd.draw(tex).to(current)();

        rnd.present();
    }

    return EXIT_SUCCESS;
}
