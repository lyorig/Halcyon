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

    const hal::font fnt { app.ttf.load_font("assets/fonts/m5x7.ttf", 144) };

    hal::texture    logo { app.window, fnt.render("Made with Halcyon") };
    hal::coordinate pos = hal::anchor::resolve(hal::anchor::center, app.window.size() / 2, logo.size());

    lyo::slider<lyo::f64> alpha { 0.0, 255.0, 1.0 };
    lyo::precise_timer    tmr { lyo::no_init };
    lyo::f64              incr { 100.0 };

    state s { up };
    bool  d { false };

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
            if (tmr() >= 4.0)
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
            {
                if (d)
                    return;

                logo = fnt.render("by lyorig");
                logo.set_opacity(lyo::cast<lyo::u8>(alpha.value()));
                pos = hal::anchor::resolve(hal::anchor::center, app.window.size() / 2, logo.size());

                s = up;
                d = true;

                incr /= -2.0;
            }

            break;
        }

        hal::texture::draw(logo).to(pos)();
    }
}

void game::start()
{
}