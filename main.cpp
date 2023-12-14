#include "game.hpp"
#include <lyo/utility.hpp>

// Current arguments:
// -xi  - Skip the intro.
// -xv  - Disable VSync.

int main(int argc, char* argv[])
{
    hq::game game { { argc, argv } };

    if (!game.app.args.has("-xi"))
        game.intro();

    game.start();

    return EXIT_SUCCESS;
}