#include <halcyon/halcyon.hpp>

int main()
{
    hal::cleanup c { hal::system::video };

    hal::window okno { "Halcyon App", { 640, 480 }, { hal::window::flags::resizeable } };
    hal::window druhe_okno { "Second one", { 480, 640 }, {} };

    hal::event evt;

    while (true)
    {
        while (evt.poll())
        {
            using enum hal::event_type;

            switch (evt.type())
            {
            case quit_requested:
                return EXIT_SUCCESS;

            case window_event:
                if (evt.window().type() == hal::events::window::closed)
                {
                    if (evt.window().window_id() == okno.id())
                        okno.release();

                    else
                        druhe_okno.release();
                }

            default:
                break;
            }
        }
    }
}
