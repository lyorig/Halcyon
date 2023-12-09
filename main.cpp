#include <halcyon/mono_app.hpp>

int main(int argc, char* argv[])
{
    hal::mono_app game { "Interloper 1.1" };

    const char* logo_text { argc == 1 ? "Sample text" : argv[1] };

    const hal::font txf { game.ttf.load_font("assets/fonts/m5x7.ttf", 72) };
    const hal::texture tex {
        game.window, txf.render(logo_text).resize(game.window.size().x * 0.5 / txf.size_text(logo_text).x)
    };

    hal::texture dlt { game.window };
    hal::color bg { hal::color::blue };
    lyo::precise_timer tmr, delta;

    delta += 1.0;

    std::size_t frames { 0 };

    game.window.renderer.set_fill(bg);
    game.mixer.mus.play("assets/ost/The Way Home.mp3", hal::infinite_loop);

    constexpr lyo::f64 FPS_update_interval { 1.0 };

    while (game.update() && !game.input().pressed(hal::button::esc)) {
        ++frames;

        if (delta() >= FPS_update_interval) {
            const auto d = lyo::u32(std::round(frames / delta()));

            dlt = txf.render(std::to_string(d));

            frames = 0;
            delta.reset();
        }

        if (game.input().pressed(hal::button::backspace))
            HAL_CONSOLE_LOG(hal::severity::info, "");

        const auto sine { std::sin(tmr()) };

        hal::texture::drawer(dlt).to({ 50, 500 })();
        hal::texture::drawer(tex).to(hal::coordinate(game.window.size() / 2 - tex.size() / 2)).scale((sine + 2.0) * 0.5).rotate(sine * 20.0)();

        HAL_CONSOLE_DRAW(txf, game.window);

        bg.r = lyo::round_cast<lyo::u8>((sine + 1.0) * 64.0);
        game.window.renderer.set_fill(bg);
    }

    return EXIT_SUCCESS;
}
