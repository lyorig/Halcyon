#include <halcyon/halcyon.hpp>
#include <halcyon/other/clipboard.hpp>

#include <lyo/utility.hpp>

#include "data.hpp"

namespace test
{
    // This test should fail.
    int assert_fail()
    {
        HAL_ASSERT(false, "This is intentional.");

        return EXIT_SUCCESS;
    }

    // Resizing a window and checking whether the event handler was notified.
    int window_resize()
    {
        constexpr hal::pixel_point new_size { 120, 120 };

        hal::cleanup c { hal::system::video };

        hal::window wnd { "HalTest Window", { 100, 100 }, { hal::window::flags::hidden } };

        hal::event_handler e;

        while (e.poll()) // Clear events.
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

    // Basic Halcyon initialization.
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

    // Passing an invalid buffer to a function expecting valid image data.
    // This test should fail.
    int invalid_buffer()
    {
        constexpr std::uint8_t data[1024] {};

        // Failure should occur here.
        const hal::surface s { hal::image::load(hal::from_memory(data)) };

        return EXIT_SUCCESS;
    }

    // Setting and getting the clipboard.
    int clipboard()
    {
        constexpr char text[] { "We can be heroes - just for one day." };

        hal::cleanup c { hal::system::video };

        hal::clipboard::set(text);

        if (!hal::clipboard::has_text() || hal::clipboard::get() != text)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    // Checking pixel colors in a 2x1 surface.
    int surface_color()
    {
        constexpr hal::color red { 255, 0, 0 }, blue { 0, 0, 255 };

        hal::surface s { hal::image::load(hal::from_memory(two_by_one)) };

        if (s[{ 0, 0 }] != red || s[{ 1, 0 }] != blue)
            return EXIT_FAILURE;

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

    if (lyo::streq(argv[1], "--assert-fail"))
        return test::assert_fail();

    else if (lyo::streq(argv[1], "--window-resize"))
        return test::window_resize();

    else if (lyo::streq(argv[1], "--basic-init"))
        return test::basic_init();

    else if (lyo::streq(argv[1], "--invalid-buffer"))
        return test::invalid_buffer();

    else if (lyo::streq(argv[1], "--clipboard"))
        return test::clipboard();

    else if (lyo::streq(argv[1], "--surface-color"))
        return test::surface_color();

    else
    {
        std::printf("Unknown test type given: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
}
