#include "game.hpp"
#include <lyo/slider.hpp>

using namespace hq;

game::game(lyo::parser&& args)
    : app { std::move(args), "HalodaQuest" }
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

    // This has to be manually timed. Then again, what other option is there?
    constexpr std::array texts {
        info { .text = "Made with Halcyon", .scale = 1.5, .hold = 3.8 },
        info { .text = "by lyorig", .hold = 2.6 },
        info { .text = "HalodaQuest", .scale = 2.5, .fade_in = 4.0, .hold = 6.0, .fade_out = 1.0, .color = hal::color::cyan }
    };

    const hal::font       fnt { app.ttf.load("assets/fonts/m5x7.ttf", 144) };
    const hal::pixel_size winhalf { app.window.size() / 2 };

    lyo::precise_timer middle_timer { lyo::no_init };

    app.mixer.music.load("assets/ost/intro_v2.mp3").fade_in(texts.front().fade_in);

    for (lyo::u8 i { 0 }; i < texts.size(); ++i)
    {
        const info& part { texts[i] };

        const hal::texture    tx { app.window, fnt.render(part.text, part.color) };
        const hal::coordinate pos = hal::anchor::resolve(hal::anchor::center, winhalf, tx.size() * part.scale);

        hal::texture::draw dw { tx };
        (void)dw.to(pos).scale(part.scale);

        const lyo::f64 hold_time { i == texts.size() - 1 ? std::min(app.mixer.music.duration() - app.mixer.music.position() - part.fade_out, part.hold - part.fade_out) : part.hold };

        hal::opacity_slider alpha { 0.0 };
        lyo::f64            opacity_incr { alpha.range() / part.fade_in };

        state dir { up };

        tx.set_opacity(alpha.min());
        HAL_DEBUG_PRINT(hal::severity::info, "Iterating: ", part.text);

        while (app.update())
        {
            alpha += opacity_incr * app.delta();
            tx.set_opacity(lyo::cast<lyo::u8>(alpha.value()));

            dw();
            HAL_CONSOLE_DRAW(fnt, app.window);

            if (app.input().pressed(hal::button::esc))
                goto GetDown;

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
                if (middle_timer() >= hold_time)
                {
                GetDown:
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

                break;
            }
        }
    Breakout:
    }

    // Mix_FreeMusic blocks until the music has finished fading
    // out, which requires a headstart in the last iteration.
    app.mixer.music.release();

    HAL_DEBUG_PRINT(hal::severity::info, "Intro finished. Welcome to ", app.window.title(), '.');
}

void game::start()
{
    const hal::font    fnt { app.ttf.load("assets/fonts/m5x7.ttf", 144) };
    const hal::texture tex { app.window, fnt.render("[menu screen]") };

    const hal::coordinate pos = hal::anchor::resolve(hal::anchor::center, app.window.size() / 2, tex.size());

    while (app.update())
    {
        hal::texture::draw { tex }.to(pos)();
        HAL_CONSOLE_DRAW(fnt, app.window);
    }
}