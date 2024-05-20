#include <iostream>
#include <thread>

#include <halcyon/events.hpp>
#include <halcyon/video.hpp>

// text_render.cpp:
// Renders the first command-line argument as text into a window.

int main(int argc, char* argv[])
{
    static_assert(hal::meta::is_correct_main<main>);

    constexpr hal::pixel_point padding { 20, 20 };

    if (argc == 1)
    {
        std::cout << "Usage: " << argv[0] << " [text]\n";
        return EXIT_FAILURE;
    }

    hal::context ctx;

    hal::system::video vid { ctx };

    hal::window         wnd { vid.make_window("Text renderer", { 100, 100 }) };
    hal::event::handler evt { vid.events };

    // Deallocate as much as we can before the main loop.
    {
        hal::renderer     rnd { wnd.make_renderer({ hal::renderer::flags::vsync }) };
        hal::ttf::context tctx;

        const hal::texture tex { rnd.make_texture(tctx.load("assets/m5x7.ttf", 16).render(argv[1]).fg(hal::palette::black)()) };

        const hal::pixel_rect draw_rect {
            padding / 2, hal::scale::width(512)(tex.size())
        };

        wnd.size(draw_rect.size + padding);
        rnd.color(hal::palette::weezer_blue);

        rnd.clear();

        rnd.render(tex).to(draw_rect)();
        rnd.present();
    }

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

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(20ms);
    }

    return EXIT_SUCCESS;
}