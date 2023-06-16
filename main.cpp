#include <halcyon/engine.hpp>
#include <halcyon/events/queue.hpp>
#include <halcyon/image_loader.hpp>
#include <halcyon/input_handler.hpp>
#include <halcyon/mixer.hpp>
#include <halcyon/ttf_engine.hpp>
#include <halcyon/window.hpp>

#include "intro.hpp"

int main(int argc, char* argv[])
{
    hal::engine eng;
    hal::mixer  mxr { eng };

    hal::window wnd { eng, "blabla", {}, wnd.fullscreen, wnd.renderer.accelerated | wnd.renderer.vsync };

    hal::input_handler inp { eng };

    hal::events::queue<hal::mixer, hal::music> x { mxr, mxr.mus };

    x.add([](hal::mixer& mxr)
        { std::puts("3sec timeout done"); },
        3.0);

    x.add([](hal::mixer& mxr)
        { std::puts("music timeout done"); },
        [](const hal::music& mus)
        { return mus.position() > 10.0; });

    mxr.mus.play("assets/ost/checkpoint.mp3");

    while (!inp.pressed(inp.esc))
    {
        wnd.present();
        x.update();

        inp.update();
    }

    return 0;
}