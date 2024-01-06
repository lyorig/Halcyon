#include <array>
#include <game.hpp>

using namespace hq;

constexpr lyo::f64 curve(lyo::f64 t)
{
    return t * (2.0 - t);
}

game::game(lyo::parser&& args)
    : m_video { m_eng }
    , m_audio { m_eng }
    , m_image { m_video, { hal::image_loader::png } }
    , m_ttf { m_video }
    , m_input { m_eng }
    , m_args { std::move(args) }
    , m_mixer { m_audio }
    , m_window { m_video, "HalodaQuest v0.1", hal::fullscreen_mode }
    , m_renderer { m_window, { hal::renderer::accelerated } }
{
}

void game::intro()
{
    if (m_args.has("-xi"))
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
        const lyo::f64   scale { 1.0 }, fade_in { 1.0 }, until, fade_out;
        const hal::color color { hal::color::white };
    };

    // This has to be manually timed. Then again, what other option is there?
    // Warning: The hold value is relative to the intro's start.
    constexpr std::array texts {
        info { .text = "Made with Halcyon", .scale = 1.5, .fade_in = 2.0, .until = 4.8, .fade_out = 0.75 },
        info { .text = "by lyorig", .until = 9.0, .fade_out = 0.6 },
        info { .text = "HalodaQuest", .scale = 2.5, .fade_in = 6.0, .until = 20.0, .fade_out = 1.5, .color = hal::color::cyan }
    };

    const hal::font       fnt { m_ttf.load("assets/m5x7.ttf", 144) };
    const hal::pixel_size winhalf { m_window.size() / 2 };

    if (!m_args.has("-xbgm"))
        m_mixer.music.load("assets/intro.mp3").fade_in(texts.front().fade_in);

    const lyo::precise_timer timer;

    for (lyo::u8 i { 0 }; i < texts.size(); ++i)
    {
        const info& part { texts[i] };

        hal::texture tx { m_renderer, fnt.render(part.text, part.color) };

        const hal::coord pos = hal::anchor::resolve(hal::anchor::center, winhalf, tx.size() * part.scale);

        hal::draw dw { tx };
        (void)dw.to(pos).scale(part.scale);

        opacity_slider alpha { 0.0 };
        alpha.set_mod(alpha.range() / part.fade_in);

        state dir { up };

        while (this->update())
        {
            tx.set_opacity(lyo::cast<hal::color::value>(alpha.update(this->delta())));

            dw(m_renderer);
            HAL_DEBUG_DRAW(m_renderer, fnt);

            if (m_input.pressed().has(hal::button::esc))
            {
                i = static_cast<lyo::u8>(texts.size() - 1);
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
                if (timer() >= part.until)
                {
                GoDown:
                    // No clue why this does what I want it to do, honestly...
                    alpha.set_mod(-alpha.range() / part.fade_out);
                    dir = down;

                    if (i == texts.size() - 1) // Calculate how fast the audio fade should be.
                        m_mixer.music.fade_out(texts.back().fade_out);
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
    m_mixer.music.release();
}

void game::start()
{
    const hal::font  fnt { m_ttf.load("assets/m5x7.ttf", 144) };
    const hal::chunk chk { m_mixer.load_sfx("assets/Button Hover.wav") };

    hal::texture tex { m_renderer, fnt.render("[X]", hal::color::red).resize({ 100, 100 }) };
    hal::coord   pos = hal::anchor::resolve(hal::anchor::center, m_window.size() / 2, tex.size());


    const hal::texture help_text { m_renderer, fnt.render("[WSAD] Move\nClick on the X to exit.").resize(0.5) };
    const hal::coord htpos {
        0,
        static_cast<hal::coord_type>(m_window.size().y - help_text.size().y) };

    bool held { false };

    constexpr hal::SDL::coord_type mod { 400.0 };
    const lyo::precise_timer       tmr;

    if (!m_args.has("-xbgm"))
        m_mixer.music.load("assets/Magic Spear.mp3").play(hal::infinite_loop);

    m_renderer.set_draw_color(0x04015c);

    while (this->update())
    {
        for (auto val : m_input.held())
        {
            switch (val)
            {
            case hal::button::W:
                pos.y -= mod * m_delta();
                break;

            case hal::button::S:
               pos.y += mod * m_delta();
                break;

            case hal::button::A:
                pos.x -= mod * m_delta();
                break;

            case hal::button::D:
                pos.x += mod * m_delta();
                break;

            default:
                break;
            }
        }

        if (hal::coord(m_input.mouse()) | pos.rect(tex.size()))
        {
            if (!held)
            {
                tex.set_color_mod(0x808080);
                chk.play();
                held = true;
            }

            if (m_input.pressed().has(hal::button::lmb))
                m_input.quit();
        }

        else if (held)
        {
            tex.set_color_mod(0xFFFFFF);
            held = false;
        }

        HAL_DEBUG_DRAW(m_renderer, fnt);

        hal::draw(tex).to(pos)(m_renderer);
        hal::draw(help_text).to(htpos)(m_renderer);
    }
}

bool game::update()
{
    m_delta.reset();
    m_renderer.present();

    return m_input.update();
}

lyo::f64 game::delta() const
{
    return m_delta();
}