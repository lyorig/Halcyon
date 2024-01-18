#include <quest/game.hpp>

int main(int argc, char* argv[])
{
    quest::game HalodaQuest { { argc, argv } };

    HalodaQuest.start();

    return EXIT_SUCCESS;
}