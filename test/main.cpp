#include <halcyon/halcyon.hpp>

int main()
{
    // hal::ttf_engine e;

    hal::font f { "assets/m5x7.ttf", 42 };

    hal::surface s { f.render("fff") };

    return 0;
}