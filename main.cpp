#include <halcyon/mono_app.hpp>
#include <iostream>

#include "halcyon/texture.hpp"

int main(int argc, char* argv[])
{
    hal::mono_app game { "Interloper 1.1" };

    // game.mixer.mus.play("assets/ost/The Way Home.mp3", hal::infinite_loop);

    const hal::font cnf { game.ttf.load_font("assets/fonts/JetBrains Mono.ttf", 72) },
        txf { game.ttf.load_font("assets/fonts/m5x7.ttf", 144) };
    const hal::texture tex { game.window, txf.render(argc == 1 ? "Made with Halcyon" : argv[1]) };

    hal::texture dlt { game.window };

    game.window.renderer.set_fill(hal::color::blue);

    lyo::precise_timer tmr, delta;
    hal::color         bg { hal::color::blue };

    while (game.update() && !game.input().pressed(hal::button::esc))
    {
        const std::string dt = std::to_string((0.01666666 / delta()) * 60.0);

        (dlt = txf.render(dt)).draw(hal::anchor::bottom_left);
        // HAL_DEBUG_PRINT(hal::severity::info, dt, " FPS");  // <-- 40FPS without, 300 with?!

        delta.reset();

        const auto sine { std::sin(tmr()) };

        tex.draw(hal::anchor::center, (sine + 2.0) * 0.5, sine * 20.0);

        bg.r = lyo::round_cast<lyo::u8>((sine + 1.0) * 64.0);
        game.window.renderer.set_fill(bg);

        HAL_CONSOLE_DRAW(cnf, game.window);
    }

    return EXIT_SUCCESS;
}