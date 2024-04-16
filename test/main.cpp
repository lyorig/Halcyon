#include <lyo/utility.hpp>

#include <halcyon/events.hpp>
#include <halcyon/video.hpp>

#include "data.hpp"

// Halcyon testing.
// A single test-runner executable that contains all tests.
// Tests are added to CTest by specifiying the appropriate command-line argument.

namespace test
{
    // Debug assertion testing.
    // This test should fail.
    int assert_fail()
    {
        // Failure should occur here.
        HAL_ASSERT(false, "This is intentional.");

        return EXIT_SUCCESS;
    }

    // Resizing a window and checking whether the event handler was notified.
    int window_resize()
    {
        constexpr hal::pixel_point new_size { 120, 120 };

        hal::context ctx;
        hal::video   vid { ctx };

        hal::window wnd { vid, "HalTest: Window resize", { 640, 480 }, { hal::window::flags::hidden } };

        hal::event::handler e { vid.events };

        while (e.poll()) // Clear events.
            ;

        wnd.size(new_size);
        e.poll();

        if (e.type() != hal::event::type::window)
            return EXIT_FAILURE;

        if (e.window().type() != hal::event::window::type::resized)
            return EXIT_FAILURE;

        if (e.window().new_point() != new_size)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    // Basic Halcyon initialization.
    int basic_init()
    {
        hal::context ctx;

        HAL_ASSERT(!hal::video::initialized(), "Video should not be initialized at this point");

        hal::video vid { ctx };

        HAL_ASSERT(hal::video::initialized(), "Video should report initialization by now");

        hal::window   wnd { vid, "HalTest: Basic init", { 640, 480 }, { hal::window::flags::hidden } };
        hal::renderer rnd { wnd, { hal::renderer::flags::accelerated } };

        hal::event::handler e { vid.events };
        e.poll();

        rnd.present();

        return EXIT_SUCCESS;
    }

    // Passing a zeroed-out buffer to a function expecting valid image data.
    // This test should fail.
    int invalid_buffer()
    {
        constexpr std::uint8_t data[1024] {};

        hal::image::context ictx { hal::image::format::png };

        // Failure should occur here.
        const hal::surface s { ictx.load(hal::from_memory(data)) };

        return EXIT_SUCCESS;
    }

    // Drawing a null texture.
    // This test should fail.
    int invalid_texture()
    {
        hal::context ctx;
        hal::video   vid { ctx };

        hal::window   wnd { vid, "HalTest: Invalid texture", { 640, 480 }, { hal::window::flags::hidden } };
        hal::renderer rnd { wnd };

        hal::texture tex;

        // Failure should occur here.
        rnd.draw(tex)();

        return EXIT_SUCCESS;
    }

    // Setting and getting the clipboard.
    int clipboard()
    {
        constexpr char text[] { "We can be heroes - just for one day." };

        hal::context ctx;
        hal::video   vid { ctx };

        vid.clipboard(text);

        if (!vid.clipboard.has_text() || vid.clipboard() != text)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    // Checking pixel colors in a 2x1 surface.
    int surface_color()
    {
        hal::image::context ictx { hal::image::format::png };

        hal::surface s { ictx.load(hal::from_memory(two_by_one)) };

        if (s[{ 0, 0 }] != hal::palette::red || s[{ 1, 0 }] != hal::palette::blue)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    // Sending a quit event and checking whether it gets caught.
    int quit_event()
    {
        hal::context ctx;
        hal::events  evt { ctx };

        hal::event::handler eh { evt };

        SDL_Event e;
        e.type = SDL_QUIT;

        ::SDL_PushEvent(&e);

        while (eh.poll())
        {
            switch (eh.type())
            {
            case hal::event::type::quit_requested:
                return EXIT_SUCCESS;

            default:
                break;
            }
        }

        HAL_PANIC("Reached unreachable point");
    }

    // Basic TTF initialization.
    int ttf_init()
    {
        hal::ttf::context tctx;

        const hal::ttf::font x { tctx.load(hal::from_file("m5x7.ttf"), 48) };
        const hal::surface   surf { x.render("I hate you for what you did - and I miss you like a little kid") };

        return EXIT_SUCCESS;
    }

    int rvalues()
    {
        hal::context c;

        hal::video { c }.clipboard("Hello from HalTest!");

        return EXIT_SUCCESS;
    }
}

int main(int argc, char* argv[])
{
    constexpr std::pair<std::string_view, lyo::func_ptr<int>> tests[] {
        { "--assert-fail", test::assert_fail },
        { "--window-resize", test::window_resize },
        { "--basic-init", test::basic_init },
        { "--invalid-buffer", test::invalid_buffer },
        { "--clipboard", test::clipboard },
        { "--surface-color", test::surface_color },
        { "--invalid-textire", test::invalid_texture },
        { "--quit-event", test::quit_event },
        { "--ttf-init", test::ttf_init },
        { "--rvalues", test::rvalues }
    };

    if (argc == 1)
    {
        std::cout << "No test type given.\n";
        return EXIT_FAILURE;
    }

    const auto iter = std::find_if(std::begin(tests), std::end(tests), [&](const auto& pair)
        { return pair.first == argv[1]; });

    if (iter == std::end(tests))
    {
        std::cout << "Invalid option specified: " << argv[1] << '\n';
        return EXIT_FAILURE;
    }

    return iter->second();
}
