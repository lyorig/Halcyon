#include <cstdio>
#include <quest/states/intro.hpp>

using namespace quest::state;

type intro::update(const input_type& input, lyo::f64 delta)
{
    (void)delta;

    if (input.pressed().contains(hal::button::esc))
    {
        // Okay, so this is downright criminal. But there's a reason!
        // The intro is a one-off class that cannot be hopped back into,
        // and there's no way to explicitly delete it from anywhere else,
        // so it sort of has to commit suicide. Again, my apologies.
        delete this;
        HAL_PRINT("Intro -> menu");
        return type::menu;
    }

    return type::none;
}