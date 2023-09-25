#include <halcyon/engine.hpp>
#include <halcyon/events/animation.hpp>
#include <halcyon/input_handler.hpp>
#include <halcyon/mixer.hpp>
#include <halcyon/ttf_engine.hpp>
#include <halcyon/window.hpp>

int main(int argc, char* argv[])
{
    hal::engine eng;
    hal::window wnd {
        eng, "Interloper v1.1", hal::fullscreen, hal::renderer::accelerated | hal::renderer::vsync
    };
    hal::mixer mxr { eng };

    mxr.mus.play("assets/ost/The Way Home.mp3", hal::infinite_loop);

    hal::ttf_engine ttf { wnd };
    const hal::font m5x7 { ttf.load_font("assets/fonts/m5x7.ttf", 72) };

    const hal::texture tex { wnd, m5x7.render("Made with Halcyon", hal::color::black) };

    hal::color color { hal::color::white };

    hal::animation anim { color, [&](hal::color& c, lyo::f64 t)
        {
            using vt = hal::color::value_type;

            c.r = static_cast<vt>(std::abs(std::sin(t * 0.5)) * lyo::f64(0x80));
        } };

    const auto size { tex.vw(50.0) };

    for (hal::input_handler inp { eng }; !inp.pressed(hal::button::esc) && !inp.should_quit(); inp.update())
    {
        anim.update();

        wnd.renderer.set_fill(color);

        tex.draw(hal::anchor::center, size);

        wnd.present();
    }

    return EXIT_SUCCESS;
}