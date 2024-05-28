#include <iostream>
#include <thread>

#include <halcyon/video.hpp>

// text_render.cpp:
// Renders the first command-line argument as text into a window.

int main(int argc, char* argv[])
{
    static_assert(hal::meta::is_correct_main<main>);

    constexpr hal::pixel_point padding { 20, 20 };
    constexpr hal::font::pt_t  font_size { 128 };

    if (argc == 1)
    {
        std::cout << "Usage: " << argv[0] << " [text]\n";
        return EXIT_FAILURE;
    }

    hal::context ctx;

    hal::system::video vid { ctx };

    hal::window         wnd { vid.make_window("Text renderer", { 100, 100 }) };
    hal::event::handler evt { vid.events };

    hal::renderer rnd { wnd.make_renderer({ hal::renderer::flags::vsync }) };
    hal::texture  tex;

    // Deallocate as much as we can before the main loop.
    {
        hal::ttf::context tctx;

        const hal::font    fnt { tctx.load("assets/m5x7.ttf", font_size) };
        const hal::surface surf { fnt.render(argv[1]).fg(hal::palette::black)() };

        HAL_PRINT("Pixel format: ", hal::to_string(surf.pixel_format()));

        tex = { rnd.make_texture(surf) };
    }

    wnd.size(tex.size() + padding);
    rnd.color(hal::palette::weezer_blue);

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

        rnd.render(tex).to(padding / 2)();
        rnd.present();
    }

    return EXIT_SUCCESS;
}