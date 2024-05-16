#include <halcyon/video.hpp>

int main(int argc, char* argv[])
{
    hal::context       ctx;
    hal::system::video vid { ctx };

    auto wnd = vid.make_window("Halcyon Structure Showcase", { 640, 480 }, { hal::window::flags::resizable });
    auto rnd = wnd.make_renderer({ hal::renderer::flags::accelerated, hal::renderer::flags::vsync });

    // We can query various things.
    HAL_PRINT("Platform: ", hal::platform());

    // For example, to enumerate displays:
    for (hal::display::id_t i { 0 }; i < vid.displays.size(); ++i)
        HAL_PRINT("Display #", hal::to_printable_int(i), ": ", vid.displays[i]);

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

            case quit_requested:
                return EXIT_SUCCESS;

            case key_pressed:
                HAL_PRINT("Button: ", hal::to_string(eh.keyboard().button()), ", Key: ", hal::to_string(eh.keyboard().key()));

                if (eh.keyboard().key() == hal::keyboard::key::esc)
                {
                    // Synthesize a quit press and push it.
                    eh.event_type(quit_requested);
                    eh.push();
                }
                break;

            case window_event:
                HAL_PRINT("Window event occurred: ", hal::to_string(eh.window().event_type()));
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