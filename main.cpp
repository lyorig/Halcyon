#include "game.hpp"
#include <lyo/utility.hpp>

// Current arguments:
// -xi  - Skip the intro.
// -xg  - Skip the game.
// -xv  - Disable VSync.

int main(int argc, char* argv[])
{
    hq::game game { { argc, argv } };

    if (!game.app.args.has("-xi"))
        game.intro();

    if (!game.app.args.has("-xg"))
        game.start();

    return EXIT_SUCCESS;
}