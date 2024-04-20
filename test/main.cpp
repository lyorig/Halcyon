#include <halcyon/event.hpp>
#include <halcyon/video.hpp>

#include "data.hpp"

// Halcyon testing.
// A single test-runner executable that contains all tests.
// Tests are added to CTest by specifiying the appropriate command-line argument.

#ifndef HAL_DEBUG_ENABLED
    #error Testing requires debugging to be enabled!
#endif

namespace test
{
    // Debug assertion testing.
    // This test should fail.
    int assert_fail()
    {
        // Failure should occur here.
        HAL_ASSERT(false, "This is intentional.");

        hal::accessor a = hal::access("amogus");

        return EXIT_SUCCESS;
    }

    // Resizing a window and checking whether the event handler was notified.
    int window_resize()
    {
        constexpr hal::pixel_point new_size { 120, 120 };

        hal::context       ctx;
        hal::video::system vid { ctx };

        hal::video::window wnd { vid, "HalTest: Window resize", { 640, 480 }, { hal::video::window::flags::hidden } };

        hal::event::handler e { vid.events };

        while (e.poll()) // Clear events.
            ;

        wnd.size(new_size);
        wnd.sync();

        while (e.poll())
        {
            if (e.event_type() == hal::event::type::window_resized)
                return EXIT_SUCCESS;
        }

        return EXIT_FAILURE;
    }

    // Basic Halcyon initialization.
    int basic_init()
    {
        hal::context ctx;

        HAL_ASSERT(!hal::video::system::initialized(), "Video should not be initialized at this point");

        hal::video::system vid { ctx };

        HAL_ASSERT(hal::video::system::initialized(), "Video should report initialization by now");

        hal::video::window   wnd { vid, "HalTest: Basic init", { 640, 480 }, { hal::video::window::flags::hidden } };
        hal::video::renderer rnd { wnd, { hal::video::renderer::flags::vsync } };

        hal::event::handler e { vid.events };
        e.poll();

        rnd.present();

        return EXIT_SUCCESS;
    }

    // Drawing a null texture.
    // This test should fail.
    int invalid_texture()
    {
        hal::context       ctx;
        hal::video::system vid { ctx };

        hal::video::window   wnd { vid, "HalTest: Invalid texture", { 640, 480 }, { hal::video::window::flags::hidden } };
        hal::video::renderer rnd { wnd };

        hal::video::texture tex;

        // Failure should occur here.
        rnd.draw(tex)();

        return EXIT_SUCCESS;
    }

    // Setting and getting the clipboard.
    int clipboard()
    {
        constexpr char text[] { "We can be heroes - just for one day." };

        hal::context       ctx;
        hal::video::system vid { ctx };

        vid.clipboard(text);

        if (!vid.clipboard.has_text() || vid.clipboard() != text)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    // Checking pixel colors in a 2x1 surface.
    int surface_color()
    {
        hal::surface s { { 2, 1 } };
        s[{ 0, 0 }] = hal::palette::red;
        s[{ 1, 0 }] = hal::palette::blue;

        if (s[{ 0, 0 }] != hal::palette::red || s[{ 1, 0 }] != hal::palette::blue)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    // Sending a quit event and checking whether it gets caught.
    int quit_event()
    {
        hal::context       ctx;
        hal::event::system evt { ctx };

        hal::event::handler eh { evt };

        SDL_Event e;
        e.type = SDL_EVENT_QUIT;

        ::SDL_PushEvent(&e);

        while (eh.poll())
        {
            switch (eh.event_type())
            {
            case hal::event::type::quit_requested:
                return EXIT_SUCCESS;

            default:
                break;
            }
        }

        HAL_PANIC("Reached unreachable point");

        return EXIT_SUCCESS;
    }

    int rvalues()
    {
        hal::context c;

        hal::video::system { c }.clipboard("Hello from HalTest!");

        return EXIT_SUCCESS;
    }
}

int main(int argc, char* argv[])
{
    constexpr std::pair<std::string_view, hal::func_ptr<int>> tests[] {
        { "--assert-fail", test::assert_fail },
        { "--window-resize", test::window_resize },
        { "--basic-init", test::basic_init },
        { "--clipboard", test::clipboard },
        { "--surface-color", test::surface_color },
        { "--invalid-textire", test::invalid_texture },
        { "--quit-event", test::quit_event },
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