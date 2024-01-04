#include <HalQ/game.hpp>
#include <array>

using namespace hq;

constexpr lyo::f64 curve(lyo::f64 t)
{
    return t * (2.0 - t);
}

game::game(lyo::parser&& args)
    : app { std::move(args), "HalodaQuest" }
{
}

void game::intro()
{
    if (app.args.has("-xi"))
        return;

    using opacity_slider = hal::static_slider<lyo::f64, hal::color::min, hal::color::max, curve>;

    enum state
    {
        up,
        middle,
        down,
    };

    struct info
    {
        const char*      text;
        const lyo::f64   scale { 1.0 }, fade_in { 1.0 }, until, fade_out { 0.5 };
        const hal::color color { hal::color::white };
    };

    // This has to be manually timed. Then again, what other option is there?
    // Warning: The hold value is relative to the intro's start.
    constexpr std::array texts {
        info { .text = "T1-MWH", .scale = 1.5, .until = 5.0 },
        info { .text = "T2-bl", .until = 10.0, .fade_out = 0.5 },
        info { .text = "T3-HQ", .scale = 2.5, .fade_in = 6.0, .until = 15.0, .fade_out = 1.5, .color = hal::color::cyan }
    };

    const hal::font       fnt { app.ttf.load("assets/m5x7.ttf", 144) };
    const hal::pixel_size winhalf { app.window.size() / 2 };

    if (!app.args.has("-xbgm"))
        app.mixer.music.load("assets/intro.mp3").fade_in(texts.front().fade_in);

    lyo::precise_timer middle_timer;

    for (lyo::u8 i { 0 }; i < texts.size(); ++i)
    {
        const info& part { texts[i] };

        hal::texture tx { app.renderer, fnt.render(part.text, part.color) };

        const hal::coord pos = hal::anchor::resolve(hal::anchor::center, winhalf, tx.size() * part.scale);

        hal::draw dw { tx };
        (void)dw.to(pos).scale(part.scale);

        opacity_slider alpha { 0.0 };
        alpha.set_mod(alpha.range() / part.fade_in);

        state dir { up };

        tx.set_opacity(alpha.min());

        while (app.update())
        {
            tx.set_opacity(lyo::cast<hal::color::value>(alpha.update(app.delta())));

            dw(app.renderer);
            HAL_DEBUG_DRAW(app.renderer, fnt);

            if (app.input.pressed().has(hal::button::esc))
            {
                i = texts.size() - 1;
                goto GoDown;
            }

            switch (dir)
            {
            case up:
                if (alpha.progress() == 1.0)
                {
                    dir = middle;
                }

                break;

            case middle:
                if (middle_timer() >= part.until)
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
                if (alpha.progress() == 0.0)
                    goto NextIter;

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

    hal::texture tex { app.renderer, fnt.render("[X]", hal::color::red).resize({ 100, 100 }) };

    hal::queued_input_handler& inp { app.input };

    hal::draw dw { tex };

    void(dw.to(hal::anchor::resolve(hal::anchor::center, app.window.size() / 2, tex.size())));
    bool held { false };

    constexpr hal::SDL::coord_type mod { 400.0 };
    const lyo::precise_timer       tmr;

    if (!app.args.has("-xbgm"))
        app.mixer.music.load("assets/Magic Spear.mp3").play(hal::infinite_loop);

    app.renderer.set_draw_color(0x04015c);

    while (app.update())
    {
        for (auto val : inp.held())
        {
            switch (val)
            {
            case hal::button::W:
                dw.dest().pos.y -= mod * app.delta();
                break;

            case hal::button::S:
                dw.dest().pos.y += mod * app.delta();
                break;

            case hal::button::A:
                dw.dest().pos.x -= mod * app.delta();
                break;

            case hal::button::D:
                dw.dest().pos.x += mod * app.delta();
                break;

            default:
                break;
            }
        }

        if (hal::SDL::FPoint(inp.mouse()) | dw.dest())
        {
            if (!held)
            {
                tex.set_color_mod(0x808080);
                chk.play();
                held = true;
            }

            if (inp.pressed().has(hal::button::lmb))
                inp.quit();
        }

        else if (held)
        {
            tex.set_color_mod(0xFFFFFF);
            held = false;
        }

        HAL_DEBUG_DRAW(app.renderer, fnt);

        dw(app.renderer);
    }
}