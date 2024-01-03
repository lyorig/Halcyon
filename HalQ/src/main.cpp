#include <HalQ/game.hpp>

int main(int argc, char* argv[])
{
    lyo::parser p { argc, argv };

    if (!p.has("-xg"))
    {
        hq::game game { std::move(p) };

        game.intro();
        game.start();
    };

    return EXIT_SUCCESS;
}
