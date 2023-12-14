#include "game.hpp"
#include <lyo/slider.hpp>

using namespace hq;

game::game(const lyo::parser& args)
    : app { args, "HalodaQuest" }
{
}

void game::intro()
{
    enum state
    {
        up,
        middle,
        down
    };

    const hal::font    fnt { app.ttf.load_font("assets/fonts/m5x7.ttf", 144) };
    const hal::texture logo { app.window, fnt.render("[logo text here]") };

    const hal::coordinate pos = hal::anchor::resolve(hal::anchor::center, app.window.size() / 2, logo.size());

    lyo::slider<lyo::f64> alpha { 0.0, 255.0, 1.0 };
    lyo::precise_timer    tmr { lyo::no_init };
    lyo::f64              incr { 100.0 };

    state s { up };

    app.mixer.music.play("assets/ost/The Way Home.mp3");

    while (app.update())
    {
        alpha += incr * app.delta();

        switch (s)
        {
        case up:
            logo.set_opacity(lyo::cast<lyo::u8>(alpha.value()));
            if (alpha.value() == alpha.max())
            {
                s = middle;
                tmr.reset();
            }

            goto SkipCheck;

        case middle:
            if (!app.mixer.music.playing())
            {
                s = down;
                incr *= -2.0;
            }

        SkipCheck:
            if (app.input().pressed(hal::button::esc))
            {
                s = down;
                incr *= -2.0;
            }

            break;

        case down:
            logo.set_opacity(lyo::cast<lyo::u8>(alpha.value()));

            if (alpha.value() == alpha.min())
                return;
            break;
        }

        hal::texture::draw(logo).to(pos)();
    }
}

void game::start()
{
}