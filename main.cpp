#include "mono_app.hpp"
#include <lyo/utility.hpp>

int main(int argc, char* argv[])
{
    const lyo::parser p { argc, argv };

    hq::mono_app game { p, "Interloper 1.1" };

    const auto logo_text = p.parse<const char*>("-text=", "Made with Halcyon");

    const hal::font txf { game.ttf.load_font("assets/fonts/m5x7.ttf", 48) };
    const hal::texture tex {
        game.window, txf.render(logo_text).resize(game.window.size().x * 0.5 / txf.size_text(logo_text).x)
    };

    hal::texture dlt { game.window };
    hal::color bg { hal::color::blue };

    const lyo::precise_timer tmr;
    lyo::precise_timer delta;

    std::size_t frames { 0 };

    game.window.renderer.set_fill(bg);
    game.mixer.mus.play("assets/ost/The Way Home.mp3", hal::infinite_loop);

    const hal::pixel_area from { { 10, game.window.size().y }, txf.render("A").size() };
    const hal::coordinate fps_pos { hal::anchor::resolve(hal::anchor::bottom_left, hal::world_area(from)) },
        tex_pos { hal::coordinate(game.window.size() / 2) };

    while (game.update() && !game.input().pressed(hal::button::esc)) {
        ++frames;

        if (game.input().pressed(hal::button::enter)) {
            const auto t = delta();
            const auto d = lyo::u32(std::round(frames / t));
            dlt = txf.render(lyo::string_from_pack(d, " FPS / ", t / frames, 's'));

            frames = 0;
            delta.reset();
        }

        const auto sine { std::sin(tmr()) };

        hal::texture::draw(dlt).to(fps_pos)();
        hal::texture::draw(tex).to(tex_pos).scale((sine + 2.0) * 0.5).rotate(sine * 10.0).anchor(hal::anchor::center)();

        HAL_CONSOLE_DRAW(txf, game.window);

        bg.r = lyo::round_cast<lyo::u8>((sine + 1.0) * 64.0);
        game.window.renderer.set_fill(bg);
    }

    return EXIT_SUCCESS;
}
