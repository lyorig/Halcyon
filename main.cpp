#include <halcyon/engine.hpp>
#include <halcyon/events/binder.hpp>
#include <halcyon/font_loader.hpp>
#include <halcyon/image_loader.hpp>
#include <halcyon/input_handler.hpp>
#include <halcyon/mixer.hpp>
#include <halcyon/spritesheet.hpp>
#include <halcyon/window.hpp>

#include "intro.hpp"

int main(int argc, char* argv[])
{
    hal::engine eng;

    hal::input_handler inp { eng };
    hal::mixer         mxr { eng };

    hal::window wnd { eng, "Halcyon", hal::fullscreen, wnd.renderer.accelerated | wnd.renderer.vsync };

    hal::image_loader imgl { wnd, imgl.jpg | imgl.jpg };
    hal::font_loader  fl { wnd };

    const hal::font   m5x7 { fl.load("assets/fonts/m5x7.ttf", 42) };
    const std::string pos { "Music position: " };

    // hq::intro(wnd, imgl, fl, mxr, inp);

    mxr.mus.play("assets/ost/magic_spear.mp3", hal::infinite_loop);

    hal::texture text { wnd };
    hal::texture sus {
        wnd, imgl.load("assets/sprites/mogus.png")
    };

    const hal::spritesheet spr {
        sus, {50, 50}
    };

    lyo::steady_timer tmr;

    while (!inp.pressed(hal::button::esc))
    {
        (text = m5x7.render(pos + std::to_string(mxr.mus.position()), hal::color::orange)).draw({ 1000, 10 });

        for (const auto& frame : spr.frames())
        {
            sus.draw(static_cast<hal::coordinate>(frame.pos * 2), frame);
        }

        HAL_CONSOLE_DRAW(m5x7, wnd);
        wnd.present();

        inp.update();
    }

    return EXIT_SUCCESS;
}