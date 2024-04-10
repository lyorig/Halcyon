#include <halcyon/halcyon.hpp>

int main()
{
    hal::cleanup c { hal::system::video };

    hal::window   okno { "Halcyon App", { 640, 480 }, { hal::window::flags::resizeable } };
    hal::renderer rend { okno, { hal::renderer::flags::accelerated, hal::renderer::flags::vsync } };

    hal::event_handler evt;

    while (true)
    {
        while (evt.poll())
        {
            using enum hal::event;

            switch (evt.type())
            {
            case quit_requested:
                return EXIT_SUCCESS;

            case window:
                if (evt.window().type() == hal::window_event::resized)
                    HAL_PRINT("Velikost okna je nyní ", evt.window().new_point());
                break;

            case key_pressed:
                if (!evt.keyboard().repeat())
                    HAL_PRINT(hal::to_string(evt.keyboard().button()), " byl stisknut");
                break;

            case mouse_pressed:
                HAL_PRINT(hal::to_string(evt.mouse_button().button()), " bylo zmáčknuto");
                break;

            default:
                break;
            }
        }
    }

    // Pokud jsme se dostali sem, něco se pokazilo.
    return EXIT_FAILURE;
}
