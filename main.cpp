#include "mono_app.hpp"
#include <lyo/utility.hpp>

int main(int argc, char* argv[])
{
    const lyo::parser p { argc, argv };

    hq::mono_app game { p, p.parse<const char*>("-windowname=", "Interloper 1.1") };

    const auto logo_text = p.parse<const char*>("-text=", "Made with Halcyon");

    const hal::font txf { game.ttf.load_font("assets/fonts/m5x7.ttf", p.parse<lyo::u32>("-fontsize=", 48)) };
    const hal::texture tex {
        game.window, txf.render(logo_text).resize(game.window.size().x * 0.5 / txf.size_text(logo_text).x)
    };

    hal::texture dlt { game.window };
    hal::color bg { hal::color::blue };

    lyo::usize frames { 0 };

    game.window.renderer.set_fill(bg);
    // game.mixer.mus.play("assets/ost/The Way Home.mp3", hal::infinite_loop);

    const hal::pixel_pos from { 10, game.window.size().y };
    const hal::coordinate fps_pos = hal::anchor::resolve(hal::anchor::bottom_left, from, txf.render("A").size()),
                          tex_pos = game.window.size() / 2;

    const lyo::precise_timer tmr;
    lyo::precise_timer delta;

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
        hal::texture::draw(tex).to(tex_pos).scale((sine + 2.0) * 0.5).anchor(hal::anchor::center)();

        HAL_CONSOLE_DRAW(txf, game.window);

        bg.r = lyo::round_cast<lyo::u8>((sine + 1.0) * 64.0);
        game.window.renderer.set_fill(bg);
    }

    return EXIT_SUCCESS;
}