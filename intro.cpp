#include "intro.hpp"

#include <halcyon/events/binder.hpp>
#include <halcyon/image_loader.hpp>
#include <halcyon/texture.hpp>
#include <halcyon/ttf_engine.hpp>
#include <halcyon/window.hpp>

using namespace halcyon;

void hq::intro(const window& wnd, const image_loader& imgl, const ttf_engine& ttf, const mixer& mix, const input_handler& input) noexcept
{
    texture logo {
        wnd, {720, 480}
    };

    wnd.render.set_target(logo);

    texture { wnd, ttf.load_text(ttf.load_font("m5x7.ttf", 72), "Made with Halcyon") }.draw({ 0, 0 });

    while (!input.pressed(SDL_SCANCODE_ESCAPE))
    {
    }

    // TODO: finish this
}