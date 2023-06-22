#include <halcyon/console.hpp>
#include <halcyon/engine.hpp>
#include <halcyon/events/binder.hpp>
#include <halcyon/font_loader.hpp>
#include <halcyon/image_loader.hpp>
#include <halcyon/input_handler.hpp>
#include <halcyon/mixer.hpp>
#include <halcyon/window.hpp>
#include <string>

#include "intro.hpp"

int main(int argc, char* argv[])
{
    hal::engine eng;

    hal::input_handler inp { eng };
    hal::mixer         mxr { eng };

    hal::window wnd { eng, "Halcyon", hal::fullscreen, wnd.renderer.accelerated | wnd.renderer.vsync };

    const hal::image_loader imgl { wnd, imgl.jpg | imgl.jpg };
    const hal::font_loader  fl { wnd };

    const hal::font   m5x7 { fl.load("assets/fonts/m5x7.ttf", 42) };
    const std::string pos { "Timer is at: " };

    hal::events::binder<hal::music> bin { inp, mxr.mus };

    bin.bind(hal::button::Q, bin.press, [](hal::music& t)
        { HAL_PRINT(hal::severity::info, "Paused music"); t.pause(); });
    bin.bind(hal::button::W, bin.press, [](hal::music& t)
        { HAL_PRINT(hal::severity::info, "Resumed music"); t.resume(); });

    CONSOLE_LOG(hal::severity::success, "Initialized. Godspeed, soldier.");

    // hq::intro(wnd, imgl, fl, mxr, inp);

    mxr.mus.play("assets/ost/checkpoint.mp3");

    while (!inp.pressed(hal::button::esc))
    {
        hal::texture {
            wnd, m5x7.textify((pos + std::to_string(mxr.mus.position())).c_str(), hal::color::orange)
        }
            .draw({ 1000, 10 });

        bin.update();

        CONSOLE_DRAW(m5x7, wnd);
        wnd.present();

        inp.update();
    }

    return EXIT_SUCCESS;
}