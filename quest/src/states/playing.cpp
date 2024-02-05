#include <quest/states/playing.hpp>

using namespace quest::state;

type playing::update(const input_type& input, lyo::f64 delta)
{
    (void)delta;

    if (input.pressed().contains(hal::button::esc))
    {
        HAL_PRINT("Playing -> paused");
        return type::paused;
    }

    return type::none;
}