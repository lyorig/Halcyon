#include "intro.hpp"

#include <halcyon/events/queue.hpp>
#include <halcyon/image_loader.hpp>
#include <halcyon/mixer.hpp>
#include <halcyon/texture.hpp>
#include <halcyon/ttf_engine.hpp>
#include <halcyon/window.hpp>

using namespace halcyon;

void hq::intro(const window& wnd, const image_loader& imgl, const ttf_engine& ttf, const mixer& mix, const input_handler& input) noexcept
{
    texture logo {
        wnd, {720, 480}
    };

    wnd.renderer.set_target(logo);

    texture { wnd, ttf.load_text(ttf.load_font("m5x7.ttf", 72), "Made with Halcyon") }.draw({ 0, 0 });

    events::queue<texture, const music> q { logo, mix.music };

    q.add([](texture& tex) {}, 0.0);
    q.add([](texture& tex) {}, 1.0);
    q.add([](texture& tex) {}, 1.0);

    while (!input.pressed(input.esc) && !q.done())
    {
    }

    // TODO: finish this
}