#include "game.hpp"
#include <halcyon/ecs.hpp>

// Current arguments:
//  -xg  - Skip the game.
//  -xi  - Skip the intro.
//  -xv  - Disable VSync.

void ecs_test()
{
    using ecs = hal::static_ecs<640, hal::pixel_size>;

    ecs e { 10 };
}

int main(int argc, char* argv[])
{
    lyo::parser p { argc, argv };

    ecs_test();

    if (!p.has("-xg"))
    {
        hq::game game { std::move(p) };

        if (!game.app.args.has("-xi"))
            game.intro();

        game.start();
    };

    return EXIT_SUCCESS;
}
