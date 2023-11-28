#include <cstdlib>
#include <halcyon/mono_app.hpp>
#include <iostream>

#include "halcyon/texture.hpp"

int main(int argc, char* argv[])
{
    if (argc == 1) {
        HAL_DEBUG_PRINT(hal::severity::error, "Missing argument. Exiting.");
        return EXIT_FAILURE;
    }

    hal::mono_app game { "Interloper 1.1" };

    game.mixer.mus.play("assets/ost/The Way Home.mp3", hal::infinite_loop);

    const hal::font txf { game.ttf.load_font("assets/fonts/m5x7.ttf", 72) };
    const hal::texture tex {
        game.window, txf.render(argv[1]).resize(game.window.size().x * 0.5 / txf.size_text(argv[1]).x)
    };

    hal::texture dlt { game.window };
    hal::color bg { hal::color::blue };
    lyo::precise_timer tmr, delta;

    game.window.renderer.set_fill(bg);

    while (game.update() && !game.input().pressed(hal::button::esc)) {
        const std::string dt = std::to_string((0.01666666 / delta()) * 60.0);

        (dlt = txf.render(dt)).draw(hal::anchor::bottom_left);
        // HAL_DEBUG_PRINT(hal::severity::info, dt, " FPS");  // Why the fuck
        // does this line double the FPS?

        delta.reset();

        const auto sine { std::sin(tmr()) };

        tex.draw(hal::anchor::center, (sine + 2.0) * 0.5, sine * 20.0);

        bg.r = lyo::round_cast<lyo::u8>((sine + 1.0) * 64.0);
        game.window.renderer.set_fill(bg);

        HAL_CONSOLE_DRAW(txf, game.window);
    }

    return EXIT_SUCCESS;
}