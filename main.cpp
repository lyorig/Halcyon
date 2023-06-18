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

    hal::input_handler inp { eng };
    hal::mixer         mxr { eng };

    hal::window wnd { eng, "Halcyon", hal::fullscreen, wnd.renderer.accelerated | wnd.renderer.vsync };

    const hal::font_loader fl { wnd };

    mxr.mus.play("assets/ost/checkpoint.mp3");

    const hal::font   m5x7 { fl.load("assets/fonts/m5x7.ttf", 42) };
    const std::string pos { "Current music position: " };

    hal::events::binder<hal::window> bin { inp, wnd };

    CONSOLE_LOG(hal::success, "Initialized. Godspeed, soldier.");

    while (!inp.pressed(inp.esc))
    {
        hal::texture {
            wnd, {m5x7, (pos + std::to_string(mxr.mus.position())).c_str(), hal::orange}
        }
            .draw({ 1000, 10 });

        bin.update();

        CONSOLE_DRAW(m5x7, wnd);
        wnd.present();

        inp.update();
    }

    return EXIT_SUCCESS;
}