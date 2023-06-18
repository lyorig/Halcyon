#include <halcyon/console.hpp>
#include <halcyon/engine.hpp>
#include <halcyon/events/binder.hpp>
#include <halcyon/font_loader.hpp>
#include <halcyon/input_handler.hpp>
#include <halcyon/mixer.hpp>
#include <halcyon/window.hpp>
#include <string>

int main(int argc, char* argv[])
{
    hal::engine eng;
    hal::mixer  mxr { eng };

    hal::window wnd { eng, "blabla", hal::fs_size, wnd.fullscreen, wnd.renderer.accelerated | wnd.renderer.vsync };

    const hal::font_loader fl { wnd };

    mxr.mus.play("assets/ost/checkpoint.mp3");

    const hal::font   fnt { fl.load("assets/fonts/m5x7.ttf", 42) };
    const std::string pos { "Current music position: " };

    hal::input_handler inp { eng };

    hal::events::binder<hal::window> bin { inp, wnd };

    bin.bind(inp.A, bin.press, [](hal::window&)
        { CONSOLE_LOG(hal::console::info, "Pressed A."); });
    bin.bind(inp.W, bin.press, [](hal::window&)
        { CONSOLE_LOG(hal::console::error, "Pressed W."); });

    for (; !inp.pressed(inp.esc); inp.update())
    {
        hal::texture {
            wnd, {fnt, (pos + std::to_string(mxr.mus.position())).c_str(), hal::orange}
        }
            .draw({ 1000, 10 });

        bin.update();

        CONSOLE_DRAW(fnt, wnd);
        wnd.present();
    }

    return EXIT_SUCCESS;
}