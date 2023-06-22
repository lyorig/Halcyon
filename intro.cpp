#include "intro.hpp"

#include <halcyon/events/queue.hpp>
#include <halcyon/font_loader.hpp>
#include <halcyon/image_loader.hpp>
#include <halcyon/input_handler.hpp>
#include <halcyon/mixer.hpp>
#include <halcyon/texture.hpp>
#include <halcyon/window.hpp>

void hq::intro(hal::window& wnd, const hal::image_loader& imgl, const hal::font_loader& fl, const hal::mixer& mxr, hal::input_handler& input) noexcept
{
    hal::texture canvas {
        wnd, {640, 480}
    };

    {
        wnd.renderer.set_target(canvas);

        const hal::font m5x7 { fl.load("assets/fonts/m5x7.ttf", 42) };

        hal::texture drawable {
            wnd, m5x7.textify("Made with Halcyon")
        };

        drawable.draw({ 0.0, 0.0 });

        drawable = m5x7.textify("by lyorig");

        drawable.draw({ 0.0, double(canvas.size().y - drawable.size().y) });

        wnd.renderer.reset_target();
    }

    const hal::coordinate pos { 0.0, 0.0 };

    mxr.mus.fade_in("assets/ost/intro.mp3", 2.0);

    while (mxr.mus.playing())
    {
        if (input.pressed(hal::button::enter))
            mxr.mus.fade_out(2.0);

        canvas.draw(pos);

        wnd.present();

        input.update();
    }
}