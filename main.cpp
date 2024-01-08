#include <game.hpp>

int main(int argc, char* argv[])
{
    hq::game game { { argc, argv } };

    game.intro_rewrite();
    game.start();

    return EXIT_SUCCESS;
}
