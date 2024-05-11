#include <iostream>
#include <thread>

#include <halcyon/event.hpp>
#include <halcyon/video.hpp>

// text_render.cpp:
// Renders the first command-line argument as text into a window.

constexpr hal::pixel_point padding { 20, 20 };

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Usage: " << argv[0] << " [text]\n";
        return EXIT_FAILURE;
    }

    hal::context ctx;

    hal::system::video vid { ctx };

    hal::window         wnd { vid.make_window("Text renderer", { 100, 100 }) };
    hal::event::handler evt { vid.events };

    // Destroy as much stuff as possible before the main loop.
    {
        hal::renderer     rnd { wnd.make_renderer({ hal::renderer::flags::vsync }) };
        hal::ttf::context tctx;

        const hal::texture tex { rnd.make_texture(tctx.load(hal::access("../assets/m5x7.ttf"), 16).render(argv[1], hal::palette::black)) };

        const hal::pixel_rect draw_rect {
            padding / 2, hal::scale::width(512)(tex.size())
        };

        wnd.size(draw_rect.size + padding);
        rnd.draw_color(hal::palette::weezer_blue);

        rnd.clear();

        rnd.draw(tex).to(draw_rect)();
        rnd.present();
    }

    while (true)
    {
        while (evt.poll())
        {
            switch (evt.event_type())
            {
                using enum hal::event::handler::type;

            case quit_requested:
                return EXIT_SUCCESS;

            default:
                break;
            }
        }

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(20ms);
    }

    return EXIT_SUCCESS;
}