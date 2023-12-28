#include "game.hpp"
#include <halcyon/ecs.hpp>

// Current arguments:
//  -xg     - Skip the game.
//  -xi     - Skip the intro.
//  -xv     - Disable VSync.
//  -xbgm   - Disable background music.

void ecs_test()
{
    using ecs = hal::static_ecs<640, hal::pixel_size>;
    using ent = hq::entity;

    ecs e { 10 };

    HAL_DEBUG_PRINT("Remaining space = ", e.memory_available(), 'B');

    ent entity { ent::init<hq::position> {} };
}

int main(int argc, char* argv[])
{
    lyo::parser p { argc, argv };

    ecs_test();

    if (!p.has("-xg"))
    {
        hq::game game { std::move(p) };

        game.intro();
        game.start();
    };

    return EXIT_SUCCESS;
}
