#include <halcyon/engine.hpp>
#include <halcyon/font_loader.hpp>
#include <halcyon/input_handler.hpp>
#include <halcyon/window.hpp>
#include <lyo/utility.hpp>

#include "scene.hpp"

int main(int argc, char* argv[])
{
    (void)argc;  // Suppress warnings.
    (void)argv;

    hal::engine eng;

    game::scene scn { eng, "Halcyon Testing Suite" };

    while (scn.update())
        ;

    return EXIT_SUCCESS;
}