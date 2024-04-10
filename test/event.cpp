#include <halcyon/halcyon.hpp>

constexpr hal::pixel_point new_size { 120, 120 };

int main()
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