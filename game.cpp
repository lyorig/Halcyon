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

    constexpr std::array texts { "Made with Halcyon", "by lyorig" };

    const hal::font fnt { app.ttf.load_font("assets/fonts/m5x7.ttf", 144) };

    hal::texture    logo { app.window, fnt.render(texts.front()) };
    hal::coordinate pos = hal::anchor::resolve(hal::anchor::center, app.window.size() / 2, logo.size());

    lyo::slider<lyo::f64> alpha { 0.0, 255.0, 0.0 };
    lyo::precise_timer    tmr { lyo::no_init };
    lyo::f64              mod { 150.0 };

    state   s { up };
    lyo::u8 idx { 0 };

    // TODO: Change this to the actual intro tune.
    app.mixer.music.play("assets/ost/The Way Home.mp3");

    while (app.update())
    {
        switch (s)
        {
        case up:
            logo.set_opacity(lyo::cast<lyo::u8>((alpha += mod * app.delta()).value()));
            if (alpha.value() == alpha.max())
            {
                s = middle;
                tmr.reset();
            }

            goto SkipCheck;

        case middle:
            if (tmr() >= 2.5)
            {
                s = down;
                mod *= 2.0;
            }

        SkipCheck:
            if (app.input().pressed(hal::button::esc))
            {
                s = down;
                mod *= 2.0;
            }

            break;

        case down:
            logo.set_opacity(lyo::cast<lyo::u8>((alpha -= mod * app.delta()).value()));

            if (alpha.value() == alpha.min())
            {
                if (++idx == texts.size())
                    return;

                logo = fnt.render(texts[idx]);
                logo.set_opacity(lyo::cast<lyo::u8>(alpha.value()));

                pos = hal::anchor::resolve(hal::anchor::center, app.window.size() / 2, logo.size());

                s = up;
                mod /= 2.0;
            }

            break;
        }

        hal::texture::draw(logo).to(pos)();
    }
}

void game::start()
{
}