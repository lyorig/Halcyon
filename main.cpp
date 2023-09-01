#include <halcyon/debug.hpp>
#include <halcyon/engine.hpp>
#include <halcyon/input_handler.hpp>
#include <halcyon/mixer.hpp>
#include <halcyon/ttf_engine.hpp>
#include <halcyon/window.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    hal::engine eng;

    hal::window wnd { eng, "Interloper v1.1", hal::fullscreen, hal::renderer::accelerated | hal::renderer::vsync };

    hal::mixer mxr { eng };

    mxr.mus.play("assets/ost/The Way Home.mp3");

    wnd.renderer.set_fill(hal::color::white);

    const hal::texture tex { wnd, hal::ttf_engine { wnd }.load_font(hal::rvalue_font, "assets/fonts/m5x7.ttf", 72).render("Made with Halcyon", hal::color::black) };

    for (hal::input_handler inp { eng }; !inp.pressed(hal::button::esc) && !inp.should_quit(); inp.update())
    {
        tex.draw(hal::anchor::center, tex.vw(50.0));

        wnd.present();
    }

    return EXIT_SUCCESS;
}