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

    sm.music.play("assets/ost/Legend of the Keeper.mp3");

    halcyon::events::binder<halcyon::texture> bin { input, soyjak };

    bin.bind(input.lmb, bin.press, [](halcyon::texture& tx)
        { puts("Pressed LMB"); });

    bin.bind(input.mmb, bin.hold, [](halcyon::texture& tx)
        { puts("Holding MMB"); });

    bin.bind(input.rmb, bin.release, [](halcyon::texture& tx)
        { puts("Released RMB"); });

    while (!input.pressed(input.esc))
    {
        soyjak.draw({ 0, 0 });
        logo.draw({ 0, 0 }, ss[0][0]);

        bin.update();

        wnd.present();
    }

    return EXIT_SUCCESS;
}