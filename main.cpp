#include <halcyon/mono_app.hpp>

int main(int argc, char* argv[])
{
    hal::mono_app game { "Interloper 1.1" };

    game.mixer.mus.play("assets/ost/The Way Home.mp3", hal::infinite_loop);

    const hal::font m5x7 { game.ttf.load_font("assets/fonts/m5x7.ttf", 144) };
    const hal::texture tex { game.window, m5x7.render("Made with Halcyon") };

    game.window.renderer.set_fill(hal::color::blue);

    lyo::precise_timer tmr;
    hal::color bg { hal::color::blue };

    while (game.update() && !game.input().pressed(hal::button::esc))
    {
        const auto sine { std::sin(tmr()) };

        tex.draw(hal::anchor::center, (sine + 2.0) * 0.5, sine * 15.0);

        bg.r = lyo::round_cast<lyo::u8>((sine + 1.0) * 64.0);
        game.window.renderer.set_fill(bg);

        HAL_CONSOLE_DRAW(m5x7, game.window);
    }

    return EXIT_SUCCESS;
}