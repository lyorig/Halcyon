#include "game.hpp"
#include <halcyon/ecs.hpp>

constexpr char help_text[] {
    "HalodaQuest, by lyorig.\n"
    "Options:\n"
    "\t-h\t- Show this message.\n"
    "\t-xg\t- Skip the game.\n"
    "\t-xi\t- Skip the intro.\n"
    "\t-xv\t- Disable VSync.\n"
    "\t-xbgm\t- Disable music."
};

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

    if (p.has("-h"))
    {
        std::cout << help_text << '\n';
        return EXIT_SUCCESS;
    }

    ecs_test();

    if (!p.has("-xg"))
    {
        hq::game game { std::move(p) };

        game.intro();
        game.start();
    };

    return EXIT_SUCCESS;
}
