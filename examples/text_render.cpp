#include <iostream>

#include <halcyon/event.hpp>
#include <halcyon/video.hpp>

constexpr hal::pixel_point padding { 20, 20 };

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Usage: " << argv[0] << " [text]\n";
        return EXIT_FAILURE;
    }

    hal::context      ctx;
    hal::ttf::context tctx;

    hal::system::video vid { ctx };

    hal::window   wnd { vid.make_window("Text renderer", { 100, 100 }) };
    hal::renderer rnd { wnd.make_renderer({ hal::renderer::flags::vsync }) };
    hal::event    evt { vid.events };

    const hal::texture tex { rnd.make_texture(tctx.load(hal::access("../assets/m5x7.ttf"), 16).render(argv[1], hal::palette::black)) };

    const hal::pixel_rect draw_rect {
        padding / 2, hal::scale::mul(10.0)(tex.size())
    };

    wnd.size(draw_rect.size + padding);
    rnd.draw_color(hal::palette::weezer_blue);

    while (true)
    {
        while (evt.poll())
        {
            switch (evt.event_type())
            {
                using enum hal::event::type;

            case quit_requested:
                return EXIT_SUCCESS;

            default:
                break;
            }
        }

        rnd.draw(tex).to(draw_rect)();
        rnd.present();
    }

    return EXIT_SUCCESS;
}