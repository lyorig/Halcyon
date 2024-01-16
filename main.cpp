#include <quest/game.hpp>

int main(int argc, char* argv[])
{
    quest::game game { { argc, argv } };

    game.intro();
    game.start();

    return EXIT_SUCCESS;
}
