#include <array>
#include <easing.hpp>
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
    , m_font { m_ttf.load("assets/m5x7.ttf", 144) } // 144pt ought to be enough for anybody.
    , m_mixer { m_audio }
    , m_window { m_video, "HalodaQuest", hal::fullscreen_mode }
    , m_renderer { m_window, { hal::renderer::accelerated } }
{
    using namespace constants;

    const hal::pixel_size window_size { m_window.size() };
    const lyo::f64        aspect_ratio { lyo::f64(window_size.y) / window_size.x };

    m_renderer.set_logical_size({ logical_width, hal::pixel_type(logical_width * aspect_ratio) });
}

template <typename T, typename Distance>
struct modifier
{
    T*       object;
    Distance dist;
    Distance base;

    lyo::func_ptr<lyo::f64, lyo::f64> func;

    Distance get(lyo::f64 x) const
    {
        return lyo::cast<Distance>(base + func(x) * dist);
    }
};

void game::intro_rewrite()
{
    if (m_args.has("-nointro"))
        return;

    struct text
    {
        hal::texture tex;
        hal::coord   pos;
    };

    enum phase : lyo::u8
    {
        mw_rotation_in,
        mw_rotation_out,
        halcyon_shift,
        by_lyorig,
        fade_out
    };

    const auto draw = [&](const text& tx)
    {
        hal::draw(tx.tex).to(tx.pos)(m_renderer);
    };

    constexpr std::array<const char[12], 3> words { "efficiency.", "precision.", "Halcyon. " };

    constexpr lyo::f64        fade_time { 1.0 };
    constexpr hal::coord_type fly_dist { 200.0 };

    const hal::pixel_size sz = m_renderer.output_size();

    // The "Made with" part.
    text made_with {
        .tex = { m_renderer, m_font.render("Made with ") },
        .pos = hal::anchor::resolve(hal::anchor::center, sz / 2, m_font.size_text(std::string("Made with ") + words.front()))
    };
    text current {
        .tex = { m_renderer, m_font.render(words.front()) },
        .pos = { made_with.pos.x + made_with.tex.size().x, made_with.pos.y - fly_dist }
    };

    modifier<hal::texture, lyo::i16> fmod {
        &current.tex,
        lyo::cast<lyo::i16>(hal::color::max / fade_time),
        hal::color::min,
        &ease::in
    };

    modifier<hal::coord_type, hal::coord_type> mmod {
        &current.pos.y,
        fly_dist,
        made_with.pos.y - fly_dist,
        &ease::in
    };

    lyo::u8 phase { mw_rotation_in };
    lyo::u8 index { 0 };

    lyo::precise_timer timer;

    while (this->update())
    {
        switch (phase)
        {
        case mw_rotation_in:
            if (timer() >= fade_time)
            {
                if (index == words.size() - 1)
                {
                    phase = halcyon_shift;

                    current.pos.y = made_with.pos.y;

                    fmod.object = &made_with.tex;
                    fmod.base   = hal::color::max;
                    fmod.dist   = -fmod.dist;

                    mmod.object = &current.pos.x;
                    mmod.dist   = -made_with.tex.size().x;
                    mmod.base   = made_with.pos.x + made_with.tex.size().x;
                    mmod.func   = &ease::quad;
                }

                else
                {
                    phase = mw_rotation_out;

                    mmod.base = made_with.pos.y;
                    mmod.func = &ease::in;

                    fmod.dist = -fmod.dist;
                    fmod.base = hal::color::max;
                    fmod.func = &ease::in;
                }

                timer -= fade_time;
            }
            break;

        case mw_rotation_out:
            if (timer() >= fade_time)
            {
                phase = mw_rotation_in;

                fmod.func = &ease::out;
                fmod.dist = -fmod.dist;
                fmod.base = hal::color::min;

                mmod.base = made_with.pos.y - fly_dist;
                mmod.func = &ease::out;

                ++index;
                current.tex.change(m_renderer, m_font.render(words[index], index == words.size() - 1 ? hal::color::cyan : hal::color::white));

                timer -= fade_time;
            }
            break;

        case halcyon_shift:
            if (timer() >= fade_time)
            {
                phase = by_lyorig;

                made_with.tex.change(m_renderer, m_font.render("by lyorig").resize(0.75));
                made_with.pos.y += current.tex.size().y - made_with.tex.size().y;

                mmod.object = &made_with.pos.x;
                mmod.base   = sz.x;
                mmod.dist   = -sz.x + current.pos.x + current.tex.size().x;
                mmod.func   = &ease::out;

                fmod.base = hal::color::max;
                fmod.dist = 0;

                timer -= fade_time;
            }
            break;

        case by_lyorig:
            if (timer() >= fade_time)
                goto Hell;
            break;
        }

        fmod.object->set_opacity(fmod.get(timer()));
        *mmod.object = mmod.get(timer());

        draw(made_with);
        draw(current);
    }
Hell:;
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