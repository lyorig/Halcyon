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

    constexpr std::array texts { "1", "2", "3", "4" };

    const hal::font fnt { app.ttf.load_font("assets/fonts/m5x7.ttf", 144) };

    hal::texture    logo { app.window, fnt.render(texts.front()) };
    hal::coordinate pos = hal::anchor::resolve(hal::anchor::center, app.window.size() / 2, logo.size());

    lyo::slider<lyo::f64> alpha { SDL_ALPHA_TRANSPARENT, SDL_ALPHA_OPAQUE, 0.0 },
        volume { 0.0, 128.0, 0.0 };
    lyo::precise_timer tmr { lyo::no_init };

    app.mixer.music.play("assets/ost/intro.mp3");
    app.mixer.music.set_volume(0);

    constexpr lyo::f64 fii { 1.5 }, foi { 0.75 }, hlt { 4.0 };
    lyo::f64           song_remaining { 0.0 }, mfi { 128 / fii };

    state   s { up };
    lyo::u8 idx { 0 };

    while (app.update())
    {
        app.mixer.music.set_volume((volume += mfi * app.delta()).value());

        switch (s)
        {
        case up:
            logo.set_opacity(lyo::cast<lyo::u8>((alpha += (SDL_ALPHA_OPAQUE / fii) * app.delta()).value()));

            if (idx == 0)
                ;

            if (alpha.value() == alpha.max())
            {
                if (idx == texts.size() - 1)
                    song_remaining = app.mixer.music.duration() - app.mixer.music.position() - foi / 2.0;

                s = middle;
                tmr.reset();
            }

            goto SkipCheck;

        case middle:
            if (tmr() >= std::max(hlt, song_remaining))
            {
                s = down;
            }

        SkipCheck:
            if (app.input().pressed(hal::button::esc))
            {
                s = down;
            }

            break;

        case down:
            logo.set_opacity(lyo::cast<lyo::u8>((alpha -= (SDL_ALPHA_OPAQUE / foi) * app.delta()).value()));

            if (idx == texts.size() - 1)
                mfi = -128 / foi;

            if (alpha.value() == alpha.min())
            {
                if (++idx == texts.size())
                    return;

                logo = fnt.render(texts[idx]);
                logo.set_opacity(lyo::cast<lyo::u8>(alpha.value()));

                pos = hal::anchor::resolve(hal::anchor::center, app.window.size() / 2, logo.size());

                s = up;
            }

            break;
        }

        HAL_CONSOLE_DRAW(fnt, app.window);
        hal::texture::draw(logo).to(pos)();
    }

    app.mixer.music.release();
}

void game::start()
{
}