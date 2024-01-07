#include <array>
#include <game.hpp>
#include <halcyon/enums/scale.hpp>

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
    , m_font { m_ttf.load("assets/m5x7.ttf", 144) } // 144pt ought to be enough for anybody.
{
}

// This is why God left us.
// TODO: Refactor!
void game::intro()
{
    if (m_args.has("-xi"))
        return;

    struct text
    {
        hal::texture tex;
        hal::coord   pos;
    };

    constexpr std::array<const char[12], 3> words { "efficiency.", "precision.", "Halcyon. " };

    constexpr lyo::f64        fade_time { 1.0 };
    constexpr hal::coord_type y_offset_base { 100.0 };

    // I calculated these bullshit formulas myself!
    const auto ease_out = [](hal::coord_type x)
    {
        return x * (x * x - 3 * x + 3);
    };

    const auto ease_in = [](hal::coord_type x)
    {
        return x * x * x;
    };

    const auto ease_in_out = [](hal::coord_type x)
    {
        lyo::f64 sqr = x * x;
        return sqr / (2.0 * (sqr - x) + 1.0);
    };

    const auto draw = [&](const text& tx)
    {
        hal::draw(tx.tex).to(tx.pos)(m_renderer);
    };

    const lyo::f64 scale { hal::scale::resolve(hal::scale::y, m_font.render("A").size(), m_window.size(), 0.1) };
    const lyo::f64 y_offset { y_offset_base * scale };

    // The "Made with" part.
    text made_with {
        .tex = { m_renderer, m_font.render("Made with ").resize(scale) },
        .pos = hal::anchor::resolve(hal::anchor::center, m_window.size() / 2, m_font.size_text("Made with efficiency.") * scale)
    };
    text current {
        .pos = { made_with.pos.x + made_with.tex.size().x }
    };

    lyo::precise_timer tmr;

    // Made with...
    for (auto iter { words.begin() }; iter != words.end(); ++iter)
    {
        const bool is_last { iter == words.end() - 1 };

        current.tex.change(m_renderer, m_font.render(*iter, is_last ? hal::color::cyan : hal::color::white).resize(scale));

        tmr.reset();

        while (this->update())
        {
            const lyo::f64 time { tmr() };
            const lyo::f64 res { ease_out(time / fade_time) };

            current.pos.y = made_with.pos.y - (1 - res) * y_offset;
            current.tex.set_opacity(lyo::cast<hal::color::value>(255 * res));

            if (time >= fade_time)
                break;

            draw(made_with);
            draw(current);
        }

        tmr.reset();

        if (is_last)
            goto Part2;

        while (this->update())
        {
            const lyo::f64 time { tmr() };
            const lyo::f64 res { ease_in(time / fade_time) };

            current.pos.y = made_with.pos.y + res * y_offset;
            current.tex.set_opacity(lyo::cast<hal::color::value>(255.0 * (1.0 - res)));

            if (time >= fade_time)
                break;

            draw(made_with);
            draw(current);
        }
    }

Part2: // Halcyon, by lyorig.
    hal::coord_type des_x { made_with.pos.x },
        dist { current.pos.x - des_x };

    while (this->update())
    {
        const lyo::f64 time { tmr() };
        const lyo::f64 res { ease_in_out(time / fade_time) };

        current.pos.x = des_x + dist * (1 - res);
        made_with.tex.set_opacity(lyo::cast<hal::color::value>(255.0 * (1.0 - res)));

        if (time >= fade_time)
            break;

        draw(made_with);
        draw(current);
    }

    // The "Made with" texture kind of gets hijacked here.
    made_with.tex.change(m_renderer, m_font.render("by lyorig").resize(scale * 0.75));
    made_with.tex.set_opacity(hal::color::max);
    made_with.pos.x = m_window.size().x;
    made_with.pos.y += (current.tex.size().y - made_with.tex.size().y) / 1.33;

    des_x = current.pos.x + current.tex.size().x;
    dist  = made_with.pos.x - des_x;

    tmr.reset();

    while (this->update())
    {
        constexpr lyo::f64 mul { 2.0 };

        const lyo::f64 time { tmr() };
        const lyo::f64 res { ease_out(time / (fade_time * mul)) };

        made_with.pos.x = des_x + dist * (1 - res);

        if (time >= fade_time * mul)
            break;

        draw(made_with);
        draw(current);
    }

    tmr.reset();

    while (this->update())
    {
        const lyo::u8 alpha { lyo::cast<lyo::u8>(255 * (1 - tmr() * 0.75)) };

        if (alpha == 0)
            break;

        made_with.tex.set_opacity(alpha);
        current.tex.set_opacity(alpha);

        draw(made_with);
        draw(current);
    }
}

void game::start()
{
    const hal::font  fnt { m_ttf.load("assets/m5x7.ttf", 144) };
    const hal::chunk chk { m_mixer.load_sfx("assets/Button Hover.wav") };

    hal::texture tex { m_renderer, fnt.render("[X]", hal::color::red).resize({ 100, 100 }) };
    hal::coord   pos = hal::anchor::resolve(hal::anchor::center, m_window.size() / 2, tex.size());

    const hal::texture help_text { m_renderer, fnt.render("[WSAD] Move\nClick on the X to exit.").resize(0.5) };
    const hal::coord   htpos {
        0,
        static_cast<hal::coord_type>(m_window.size().y - help_text.size().y)
    };

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