#include "game.hpp"
#include <array>

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
        const char*      text;
        const lyo::f64   scale { 1.0 }, fade_in { 1.0 }, hold { 4.0 }, fade_out { 0.5 };
        const hal::color color { hal::color::white };
    };

    // This has to be manually timed. Then again, what other option is there?
    constexpr std::array texts {
        info { .text = "Made with Halcyon", .scale = 1.5, .hold = 3.85 },
        info { .text = "by lyorig", .hold = 2.4, .fade_out = 0.5 },
        info { .text = "HalodaQuest", .scale = 2.5, .fade_in = 5.0, .hold = 6.5, .fade_out = 1.5, .color = hal::color::cyan }
    };

    const hal::font       fnt { app.ttf.load("assets/m5x7.ttf", 144) };
    const hal::pixel_size winhalf { app.window.size() / 2 };

    lyo::precise_timer middle_timer { lyo::no_init };

    app.mixer.music.load("assets/intro.mp3").fade_in(texts.front().fade_in).sync();

    for (lyo::u8 i { 0 }; i < texts.size(); ++i)
    {
        const info& part { texts[i] };

        hal::texture tx { app.window, fnt.render(part.text, part.color) };

        const hal::coordinate pos = hal::anchor::resolve(hal::anchor::center, winhalf, tx.size() * part.scale);

        hal::texture::draw dw { tx };
        (void)dw.to(pos).scale(part.scale);

        const lyo::f64 hold_time { i == texts.size() - 1 ? std::min(app.mixer.music.duration() - app.mixer.music.position() - part.fade_out, part.hold - part.fade_out) : part.hold };

        hal::opacity_slider alpha { 0.0 };
        alpha.set_mod(alpha.range() / part.fade_in);

        state dir { up };

        tx.set_opacity(alpha.min());

        while (app.update())
        {
            tx.set_opacity(lyo::cast<lyo::u8>(alpha.update(app.delta())));

            dw();
            HAL_DEBUG_DRAW(app.window, fnt);

            if (app.input.pressed(hal::button::esc))
            {
                i = texts.size() - 1;
                goto GoDown;
            }

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
                GoDown:
                    // No clue why this does what I want it to do, honestly...
                    alpha.set_mod(-alpha.range() / part.fade_out);
                    dir = down;

                    if (i == texts.size() - 1) // Calculate how fast the audio fade should be.
                        app.mixer.music.fade_out(texts.back().fade_out);
                    else
                        alpha.set_mod(-alpha.range() / part.fade_out);
                }

                break;

            case down:
                if (alpha.value() == alpha.min())
                {
                    goto NextIter;
                }

                break;
            }
        }
    NextIter:;
    }

    // Mix_FreeMusic blocks until the music has finished fading
    // out, which requires a headstart in the last iteration.
    app.mixer.music.release();
}

void game::start()
{
    const hal::font  fnt { app.ttf.load("assets/m5x7.ttf", 144) };
    const hal::chunk chk { app.mixer.load_sfx("assets/Button Hover.wav") };

    hal::texture        tex { app.window, fnt.render("[X]", hal::color::orange).resize({ 100, 100 }) };
    hal::input_handler& inp { app.input };
    hal::texture::draw  dw { tex };
    void(dw.to(hal::anchor::resolve(hal::anchor::center, app.window.size() / 2, tex.size())));

    bool held = false;

    app.mixer.music.load("assets/Magic Spear.mp3").play(hal::infinite_loop).sync();

    while (app.update() && !inp.pressed(hal::button::esc))
    {
        if (inp.pressed(hal::button::lmb))
            HAL_DEBUG_PRINT("Mouse click at ", inp.mouse());

        if (hal::SDL::FPoint(inp.mouse()) | dw.dest())
        {
            if (!held)
            {
                tex.set_color_mod(0x808080);
                chk.play();
                held = true;
            }

            if (inp.pressed(hal::button::lmb))
                inp.quit();
        }

        else if (held)
        {
            tex.set_color_mod(0xFFFFFF);
            held = false;
        }

        dw();
        HAL_DEBUG_DRAW(app.window, fnt);
    }
}