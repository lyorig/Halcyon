#include <quest/game.hpp>

int main(int argc, char* argv[])
{
    hq::game game { { argc, argv } };

    game.intro();
    game.start();

    return EXIT_SUCCESS;
}
