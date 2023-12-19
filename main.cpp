#include "game.hpp"

// Current arguments:
// -xi  - Skip the intro.
// -xg  - Skip the game.
// -xv  - Disable VSync.

int main(int argc, char* argv[])
{
    hq::game game { { argc, argv } };

    HAL_DEBUG_PANIC("No reason really.");

    if (!game.app.args.has("-xi"))
        game.intro();

    if (!game.app.args.has("-xg"))
        game.start();

    return EXIT_SUCCESS;
}