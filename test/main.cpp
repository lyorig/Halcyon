#include <halcyon/halcyon.hpp>

constexpr hal::pixel_point new_size { 120, 120 };

namespace test
{
    int window_resize()
    {
        hal::cleanup c { hal::system::video };

        hal::window wnd { "HalTest Window", { 100, 100 }, { hal::window::flags::hidden } };

        hal::event_handler e;

        while (e.poll()) // clear events
            ;

        wnd.size(new_size);
        e.poll();

        if (e.type() != hal::event::window_event)
            return EXIT_FAILURE;

        if (e.window().type() != hal::window_event::resized)
            return EXIT_FAILURE;

        if (e.window().new_point() != new_size)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int basic_init()
    {
        hal::cleanup c { hal::system::video };

        hal::window   wnd { "HalTest Window", { 100, 100 }, { hal::window::flags::hidden } };
        hal::renderer rnd { wnd, { hal::renderer::flags::accelerated, hal::renderer::flags::vsync } };

        hal::event_handler e;
        e.poll();

        rnd.present();

        return EXIT_SUCCESS;
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::puts("No test type given.");
        return EXIT_FAILURE;
    }

    if (lyo::streq(argv[1], "--window-resize"))
        return test::window_resize();

    if (lyo::streq(argv[1], "--basic-init"))
        return test::basic_init();

    else
    {
        std::printf("Unknown test type given: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
}