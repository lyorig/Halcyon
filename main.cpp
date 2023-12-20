#include "game.hpp"
#include <halcyon/ecs/ecs.hpp>

// Current arguments:
// -xi  - Skip the intro.
// -xg  - Skip the game.
// -xv  - Disable VSync.

constexpr void ecs_test();

int main(int argc, char* argv[])
{
    lyo::parser p { argc, argv };

    if (p.has("-testecs"))
        ecs_test();

    else
    {
        hq::game game { std::move(p) };

        if (!game.app.args.has("-xi"))
            game.intro();

        if (!game.app.args.has("-xg"))
            game.start();

        return EXIT_SUCCESS;
    };
}

constexpr void ecs_test()
{
    constexpr hal::component::index num_coord { 10 }, num_pixel { 20 };
    constexpr std::size_t           total_size { num_coord * sizeof(hal::coordinate) + num_pixel * sizeof(hal::pixel_size) };

    hal::ecs<2, total_size> ecs;

    ecs.add<hal::coordinate>(num_coord);

    assert(ecs.space<hal::coordinate>() == num_coord);
    assert(ecs.memory_used() == num_coord * sizeof(hal::coordinate));
    assert(ecs.registered() == 1);

    ecs.add<hal::pixel_size>(num_pixel);

    assert(ecs.space<hal::pixel_size>() == num_pixel);
    assert(ecs.memory_used() == total_size);
    assert(ecs.registered() == 2);
}