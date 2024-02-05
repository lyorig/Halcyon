#include <quest/states/paused.hpp>

using namespace quest::state;

type paused::update(const input_type& input, lyo::f64 delta)
{
    (void)delta;

    if (input.pressed().contains(hal::button::esc))
    {
        HAL_PRINT("Paused -> playing");
        return type::playing;
    }

    return type::none;
}