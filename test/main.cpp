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

        hal::context       ctx;
        hal::system::video vid { ctx };

        hal::window wnd { vid.make_window("HalTest: Window resize", { 640, 480 }, { hal::window::flags::hidden }) };

        hal::event::handler e { vid.events };

        while (e.poll()) // Clear events.
            ;

        wnd.size(new_size);
        e.poll();

        if (e.event_type() != hal::event::type::window_event)
            return EXIT_FAILURE;

        if (e.window().event_type() != hal::event::window_event::type::resized)
            return EXIT_FAILURE;

        if (e.window().point() != new_size)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    // Basic Halcyon initialization.
    int basic_init()
    {
        hal::context ctx;

        HAL_ASSERT(!hal::system::video::initialized(), "Video should not be initialized at this point");

        hal::system::video vid { ctx };

        HAL_ASSERT(hal::system::video::initialized(), "Video should report initialization by now");

        hal::window   wnd { vid.make_window("HalTest: Basic init", { 640, 480 }, { hal::window::flags::hidden }) };
        hal::renderer rnd { wnd.make_renderer() };

        hal::event::handler e { vid.events };
        e.poll();

        rnd.present();

        return EXIT_SUCCESS;
    }

    // Setting and getting the clipboard.
    int clipboard()
    {
        constexpr char text[] { "We can be heroes - just for one day." };

        hal::context       ctx;
        hal::system::video vid { ctx };

        vid.clipboard(text);

        if (!vid.clipboard.has_text() || vid.clipboard() != text)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    // Checking pixel colors in a 2x1 surface.
    int surface_color()
    {
        hal::image::context ictx { hal::image::init_format::png };

        hal::surface s { ictx.load(hal::access(png_2x1)) };

        if (s[{ 0, 0 }].color() != hal::palette::red || s[{ 1, 0 }].color() != hal::palette::blue)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    // Sending a quit event and checking whether it gets caught.
    int events()
    {
        hal::context        ctx;
        hal::system::events evt { ctx };

        hal::event::handler eh { evt };

        using enum hal::event::type;

        while (eh.poll())
            ;

        eh.event_type(quit_requested);
        eh.push();

        if (!(eh.poll() && eh.event_type() == quit_requested))
            return EXIT_FAILURE;

        constexpr std::string_view text { "aaaaaaaaaabbbbbbbbbbccccccccccd" };

        static_assert(text.size() <= hal::event::text_input_event::max_size());

        while (eh.poll())
            ;

        eh.event_type(hal::event::type::text_input);
        eh.text_input().text(text);
        eh.push();

        if (!(eh.poll() && eh.event_type() == text_input && eh.text_input().text() == text))
        {
            HAL_PRINT(hal::to_string(eh.event_type()));
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    // Basic TTF initialization.
    int ttf_init()
    {
        {
            hal::ttf::context tctx;

            if (!hal::ttf::context::initialized())
                return EXIT_FAILURE;
        }

        if (hal::ttf::context::initialized())
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int rvalues()
    {
        hal::context c;

        hal::system::video { c }.clipboard("Hello from HalTest!");

        return EXIT_SUCCESS;
    }

    int scaler()
    {
        constexpr hal::pixel_point src { 50, 100 };
        constexpr hal::pixel_point dst { 100, 200 };

        if (hal::scale::width(100)(src) != dst)
            return EXIT_FAILURE;

        if (hal::scale::height(200)(src) != dst)
            return EXIT_FAILURE;

        if (hal::scale::mul(2.0)(src) != dst)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int outputter()
    {
        hal::surface s { { 2, 2 } };

        s[{ 0, 0 }].color(0xF25022);
        s[{ 1, 0 }].color(0x7FBA00);
        s[{ 0, 1 }].color(0x00A4EF);
        s[{ 1, 1 }].color(0xFFB900);

        s.save(hal::output("DontSueMeDaddyGates.bmp"));

        std::byte buf[1000];

        s.save(hal::output(buf));

        return EXIT_SUCCESS;
    }

    int png_check()
    {
        hal::image::context ictx { hal::image::init_format::png };

        if (ictx.query(hal::access(png_2x1)) != hal::image::load_format::png)
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    int metaprogramming()
    {
        using ret_t    = std::string;
        using first_t  = std::tuple<int, double, short>;
        using second_t = std::vector<double>;

        using list = hal::meta::type_list<first_t, second_t>;

        if constexpr (!(std::is_same_v<first_t, list::at<0>>
                          && std::is_same_v<second_t, list::at<1>>))
            return EXIT_FAILURE;

        using info = hal::meta::func_info<ret_t(first_t, second_t)>;

        if constexpr (!(std::is_same_v<ret_t, info::return_type>
                          && std::is_same_v<first_t, info::args::at<0>>
                          && std::is_same_v<second_t, info::args::at<1>>))
            return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    // Passing a zeroed-out buffer to a function expecting valid image data.
    // This test should fail.
    int invalid_buffer()
    {
        constexpr std::uint8_t data[1024] {};

        hal::image::context ictx { hal::image::init_format::png };

        // Failure should occur here.
        const hal::surface s { ictx.load(hal::access(data)) };

        return EXIT_SUCCESS;
    }

    // Drawing a null texture.
    // This test should fail.
    int invalid_texture()
    {
        hal::context       ctx;
        hal::system::video vid { ctx };

        hal::window   wnd { vid.make_window("HalTest: Invalid texture", { 640, 480 }, { hal::window::flags::hidden }) };
        hal::renderer rnd { wnd.make_renderer() };

        hal::texture tex;

        // Failure should occur here.
        rnd.draw(tex)();

        return EXIT_SUCCESS;
    }

    // Accessing an invalid event.
    // This test should fail.
    int invalid_event()
    {
        hal::context        ctx;
        hal::system::events sys { ctx };

        hal::event::handler eh { sys };
        eh.text_input().text("amogus sus").window_id(69);

        return EXIT_SUCCESS;
    }
}

int main(int argc, char* argv[])
{
    static_assert(hal::compile_settings::debug_enabled, "HalTest requires debug mode to be enabled");

    constexpr std::pair<std::string_view, hal::func_ptr<int>> tests[] {
        { "--assert-fail", test::assert_fail },
        { "--window-resize", test::window_resize },
        { "--basic-init", test::basic_init },
        { "--clipboard", test::clipboard },
        { "--surface-color", test::surface_color },
        { "--events", test::events },
        { "--ttf-init", test::ttf_init },
        { "--rvalues", test::rvalues },
        { "--scaler", test::scaler },
        { "--outputter", test::outputter },
        { "--png-check", test::png_check },
        { "--metaprogramming", test::metaprogramming },
        { "--invalid-buffer", test::invalid_buffer },
        { "--invalid-texture", test::invalid_texture },
        { "--invalid-event", test::invalid_event }
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