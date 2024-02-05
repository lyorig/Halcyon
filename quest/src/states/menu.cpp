#include <quest/states/menu.hpp>

using namespace quest::state;

type menu::update(const input_type& input, lyo::f64 delta)
{
    (void)input;
    (void)delta;

    if (input.pressed().contains(hal::button::enter))
    {
        HAL_PRINT("Menu -> playing");
        return type::playing;
    }

    return type::none;
}