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
        down,
    };

    struct info
    {
        const char* text;
        lyo::f64    scale { 1.0 }, fade_in { 1.0 }, hold { 4.0 }, fade_out { 0.5 };
        hal::color  color { hal::color::white };
    };

    constexpr std::array texts { // This has to be manually timed. Then again, what other option is there?
        info { .text = "Made with Halcyon", .hold = 3.8 },
        info { .text = "by lyorig", .scale = 0.75, .hold = 2.6 },
        info { .text = "HalodaQuest", .scale = 2.0, .fade_in = 4.0, .hold = 6.0, .fade_out = 1.0, .color = hal::color::cyan }
    };

    const hal::font       fnt { app.ttf.load_font("assets/fonts/m5x7.ttf", 144) };
    const hal::pixel_size winhalf { app.window.size() / 2 };

    lyo::precise_timer middle_timer { lyo::no_init };

    app.mixer.music.load("assets/ost/intro_v2.mp3").fade_in(texts.front().fade_in);

    for (lyo::u8 i { 0 }; i < texts.size(); ++i)
    {
        const info& part { texts[i] };

        HAL_DEBUG_PRINT(hal::severity::info, "Iterating: ", part.text);

        const hal::texture    tx { app.window, fnt.render(part.text, part.color) };
        const hal::coordinate pos = hal::anchor::resolve(hal::anchor::center, winhalf, tx.size() * part.scale);

        hal::opacity_slider alpha { 0.0 };
        lyo::f64            opacity_incr { alpha.range() / part.fade_in };

        state dir { up };

        while (app.update())
        {
            alpha += opacity_incr * app.delta();
            tx.set_opacity(lyo::cast<lyo::u8>(alpha.value()));

            hal::texture::draw { tx }.to(pos).scale(part.scale)();
            HAL_CONSOLE_DRAW(fnt, app.window);

            // HAL_DEBUG_PRINT(hal::severity::info, "New opacity is ", alpha.value());

            switch (dir)
            {
            case up:
                if (alpha.value() == alpha.max())
                {
                    middle_timer.reset();
                    dir = middle;
                }

                break;

            case middle:
                if (middle_timer() >= part.hold)
                {
                    opacity_incr = -alpha.range() / part.fade_out;
                    dir = down;

                    if (i == texts.size() - 1)
                        app.mixer.music.fade_out(part.fade_out);
                }

                break;

            case down:
                if (alpha.value() == alpha.min())
                {
                    goto Breakout;
                }
            }
        }
    Breakout:
    }

    app.mixer.music.release();
}

void game::start()
{
    const hal::font    fnt { app.ttf.load_font("assets/fonts/m5x7.ttf", 144) };
    const hal::texture tex { app.window, fnt.render("[menu screen]") };

    const hal::coordinate pos = hal::anchor::resolve(hal::anchor::center, app.window.size() / 2, tex.size());

    while (app.update())
    {
        hal::texture::draw { tex }.to(pos)();
        HAL_CONSOLE_DRAW(fnt, app.window);
    }
}