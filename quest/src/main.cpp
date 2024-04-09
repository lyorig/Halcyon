#include <halcyon/halcyon.hpp>

int main()
{
    hal::cleanup c { hal::system::video };

    hal::window okno { "Halcyon App", { 640, 480 }, { hal::window::flags::resizeable } };
}
