#include <lyoSTL/math.hpp>
#include <quest/game.hpp>

int main(int argc, char* argv[])
{
    quest::game game { { argc, argv } };

    game.intro();
    game.start();

    for (int i { 0 }; i < 10; ++i)
    {
        HAL_DEBUG_PRINT(i, ": ", lyo::factorial(i));
    }

    return EXIT_SUCCESS;
}
