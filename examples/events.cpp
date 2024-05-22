#include <halcyon/video.hpp>

// events.cpp:
// An example event loop in a game.

constexpr hal::pixel_point window_size_mod { 20, 20 };

int main(int, char*[])
{
    static_assert(hal::meta::is_correct_main<main>);

    using wf = hal::window::flags;
    using rf = hal::renderer::flags;

    hal::context       ctx;
    hal::system::video vid { ctx };

    hal::window   wnd { vid.make_window("Halcyon Structure Showcase", { 640, 480 }, { wf::resizable }) };
    hal::renderer rnd { wnd.make_renderer({ rf::accelerated, rf::vsync }) };

    hal::event::handler eh { vid.events };

    // The application's main loop starts here.
    while (true) // As long as the program is running...
    {
        while (eh.poll()) // As long as there are events to process...
        {
            switch (eh.event_type())
            {
                using enum hal::event::type;

            case terminated:
                HAL_PRINT("Program has been terminated!");
                // intentional fallthrough

            case quit_requested:
                return EXIT_SUCCESS;

            case key_pressed:
                switch (eh.keyboard().key())
                {
                    using enum hal::keyboard::key;

                case esc: // Quit.
                    eh.event_type(quit_requested);
                    eh.push();
                    break;

                case W: // Increase window size.
                    wnd.size(wnd.size() + window_size_mod);
                    break;

                case S: // Decrease window size.
                    wnd.size(wnd.size() - window_size_mod);
                    break;

                case C: // Paint the background blue.
                    rnd.color(hal::palette::weezer_blue);
                    break;

                default:
                    HAL_PRINT("Unhandled key press: ",
                        hal::to_string(eh.keyboard().key()), eh.keyboard().repeat() ? " [REPEAT]" : "");
                }
                break;

            case key_released:
                switch (eh.keyboard().key())
                {
                    using enum hal::keyboard::key;

                case C:
                    rnd.color(hal::palette::black);
                    break;

                default:
                    break;
                }
                break;

            case window_event:
                HAL_PRINT("Window event occurred: ", hal::to_string(eh.window().event_type()));
                break;

            case display_event:
                HAL_PRINT("Display event occurred: ", hal::to_string(eh.display().event_type()));
                break;

            default:
                if (auto tp = eh.event_type(); tp != hal::event::type::mouse_moved)
                    HAL_PRINT("Unhandled event occurred: ", hal::to_string(tp));
                break;
            }
        }

        rnd.present();
    }

    return EXIT_FAILURE;
}