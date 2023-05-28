#include "halcyon/engine.hpp"
#include "halcyon/events/binder.hpp"
#include "halcyon/image_loader.hpp"
#include "halcyon/mixer.hpp"
#include "halcyon/spritesheet.hpp"
#include "halcyon/texture.hpp"
#include "halcyon/ttf_engine.hpp"
#include "halcyon/window.hpp"

int main(int argc, char* argv[])
{
    halcyon::engine engine;

    halcyon::window wnd {
        engine, "lyoEngine v0.1", {},
        halcyon::window::fullscreen, halcyon::renderer::accelerated | halcyon::renderer::vsync
    };

    halcyon::image_loader img { engine, halcyon::image_loader::png };
    halcyon::mixer        sm { engine, halcyon::mixer::mp3, 48000, 16 };
    halcyon::ttf_engine   te { engine };

    halcyon::chunk chk { sm.load_sfx("assets/sfx/pistol_shot.wav") };

    halcyon::texture soyjak {
        wnd, img.load_image("assets/sprites/soyjaks.png")
    };

    halcyon::texture logo {
        wnd, te.load_text(te.load_font("assets/fonts/m5x7.ttf", 48), "lyoEngine v0.1 - made with Halcyon", 0x00FFFF)
    };

    halcyon::spritesheet ss {
        logo, {100, logo.size().y}
    };

    const halcyon::input_handler& input { wnd.input() };

    sm.mus.play("assets/ost/Legend of the Keeper.mp3");

    halcyon::events::binder<halcyon::texture> bin { input, soyjak };

    bin.bind(SDL_SCANCODE_Q, bin.press, [](halcyon::texture& tx)
        { tx.set_opacity(0); });

    bin.bind(SDL_SCANCODE_W, bin.press, [](halcyon::texture& tx)
        { tx.set_opacity(64); });

    bin.bind(SDL_SCANCODE_E, bin.press, [](halcyon::texture& tx)
        { tx.set_opacity(128); });

    bin.bind(SDL_SCANCODE_R, bin.press, [](halcyon::texture& tx)
        { tx.set_opacity(196); });

    while (!input.pressed(SDL_SCANCODE_ESCAPE))
    {
        soyjak.draw({ 0, 0 });
        logo.draw({ 0, 0 }, ss[0][0]);

        bin.update();

        wnd.present();
    }

    return EXIT_SUCCESS;
}