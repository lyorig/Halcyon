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

    constexpr std::array texts {
        info { .text = "Made with Halcyon" },
        info { .text = "by lyorig", .scale = 0.75, .hold = 2.5 },
        info { .text = "HalodaQuest", .scale = 2.0, .fade_in = 4.0, .fade_out = 1.0, .color = hal::color::cyan }
    };

    const hal::font fnt { app.ttf.load_font("assets/fonts/m5x7.ttf", 144) };

    hal::texture    logo { app.window, fnt.render(texts.front().text, texts.front().color) };
    hal::coordinate pos = hal::anchor::resolve(hal::anchor::center, app.window.size() / 2, logo.size() * texts.front().scale);

    lyo::slider<lyo::f64> alpha { SDL_ALPHA_TRANSPARENT, SDL_ALPHA_OPAQUE, 0.0 },
        volume { 0.0, MIX_MAX_VOLUME, 0.0 };
    lyo::precise_timer tmr { lyo::no_init };

    logo.set_opacity(alpha.value());

    app.mixer.music.play("assets/ost/intro.mp3");
    app.mixer.music.set_volume(volume.value());

    constexpr lyo::f64 music_fade_time { 1.5 };

    lyo::f64 hold_time { texts.front().hold },
        music_fade_increment { 128 / music_fade_time };

    state   s { up };
    lyo::u8 idx { 0 };

    while (app.update())
    {
        app.mixer.music.set_volume((volume += music_fade_increment * app.delta()).value());
        const info& i { texts[idx] };

        HAL_CONSOLE_DRAW(fnt, app.window);
        hal::texture::draw(logo).to(pos).scale(i.scale)();

        switch (s)
        {
        case up:
            logo.set_opacity(lyo::cast<lyo::u8>((alpha += (alpha.max() / i.fade_in) * app.delta()).value()));

            if (alpha.value() == alpha.max())
            {
                if (idx == texts.size() - 1)
                {
                    hold_time = app.mixer.music.duration() - app.mixer.music.position() - i.fade_out;
                    HAL_DEBUG_PRINT(hal::severity::info, "Song remaining = ", hold_time);
                }

                else
                    hold_time = i.hold;

                s = middle;
                tmr.reset();
            }

            goto SkipCheck;

        case middle:
            if (tmr() >= hold_time)
            {
                if (idx == texts.size() - 1)
                {
                    music_fade_increment = -128 / i.fade_out;
                }

                s = down;
            }

        SkipCheck:
            if (app.input().pressed(hal::button::esc))
            {
                if (idx == texts.size() - 1)
                {
                    music_fade_increment = -128 / i.fade_out;
                }

                s = down;
            }

            break;

        case down:
            logo.set_opacity(lyo::cast<lyo::u8>((alpha -= (SDL_ALPHA_OPAQUE / i.fade_out) * app.delta()).value()));

            if (alpha.value() == alpha.min())
            {
                if (idx == texts.size() - 1)
                {
                    if (volume.value() == volume.min())
                        return;
                }

                else
                {
                    const info& newi { texts[++idx] };

                    logo = fnt.render(newi.text, newi.color);
                    logo.set_opacity(lyo::cast<lyo::u8>(alpha.value()));

                    pos = hal::anchor::resolve(hal::anchor::center, app.window.size() / 2, logo.size() * newi.scale);

                    s = up;
                }
            }

            break;
        }
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