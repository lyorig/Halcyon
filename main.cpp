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
    halcyon::mixer        mxr { engine, halcyon::mixer::mp3, 48000, 16 };
    halcyon::ttf_engine   te { engine };

    halcyon::chunk chk { mxr.load_sfx("assets/sfx/pistol_shot.mp3") };

    halcyon::texture soyjak {
        wnd, img.load_image("assets/sprites/amogus.png")
    };

    halcyon::texture logo {
        wnd, te.load_text(te.load_font("assets/fonts/m5x7.ttf", 48), "lyoEngine v0.1 - made with Halcyon", 0x00FFFF)
    };

    const halcyon::input_handler& input { wnd.input() };

    mxr.music.play("assets/ost/checkpoint.mp3", 69);

    halcyon::events::binder<halcyon::music> bin { input, mxr.music };

    bin.bind(input.lmb, bin.press, [](halcyon::music& mus)
        { mus.jump(50.0); });

    bin.bind(input.mmb, bin.hold, [](halcyon::music& mus)
        { mus.jump(100.0); });

    bin.bind(input.rmb, bin.release, [](halcyon::music& mus)
        { mus.jump(150.0); });

    while (!input.pressed(input.esc))
    {
        soyjak.draw({ 0, 0 });
        logo.draw({ 0, 0 });

        bin.update();

        wnd.present();
    }

    return EXIT_SUCCESS;
}