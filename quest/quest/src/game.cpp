#include <array>
#include <quest/constants.hpp>
#include <quest/easing.hpp>
#include <quest/game.hpp>
#include <thread>

using namespace quest;

game::game(lyo::parser&& args)
    : m_video { m_eng }
    , m_audio { m_eng }
    , m_image { m_video, { hal::image_loader::png } }
    , m_ttf { m_video }
    , m_input { m_eng }
    , m_args { std::move(args) }
    , m_font { this->load_appx_font("assets/VCR.ttf", 2.5) }
    , m_mixer { m_audio }
    , m_window { m_video, "HalodaQuest", hal::fullscreen_mode }
    , m_renderer { m_window, { hal::renderer::accelerated } }
    , m_state { new state::intro }
{
    using namespace constants;

    const hal::pixel_point dsz {
        m_video.display_at(m_window.display_index()).size()
    };
    const lyo::f64 aspect_ratio { lyo::f64(dsz.x) / dsz.y };
    m_renderer.set_size({ lyo::cast<hal::pixel_t>(render_height * aspect_ratio), render_height });

    HAL_PRINT("Render size set at ", m_renderer.size());
}

template <typename T, typename Distance>
struct modifier
{
    T* object;

    Distance dist;
    Distance base;

    lyo::func_ptr<lyo::f64, lyo::f64> func;

    Distance get(lyo::f64 x) const
    {
        return lyo::cast<Distance>(base + func(x) * dist);
    }
};

void game::intro()
{
    if (m_args.has("-nointro"))
        return;

    struct text
    {
        hal::texture     tex;
        hal::coord_point pos;
    };

    enum phase : lyo::u8
    {
        mw_rotation_in,
        mw_rotation_out,
        halcyon_shift,
        by_lyorig,
        fade_out
    };

    const auto draw = [this](const text& tx)
    {
        m_renderer.draw(tx.tex).to(tx.pos)();
    };

    constexpr std::array<const char[12], 3> words { "efficiency.", "precision.", "Halcyon. " };

    constexpr lyo::f64 fade_time { 1.0 };

    const hal::font font { this->load_appx_font("assets/m5x7.ttf", "Made with Halcyon", 60.0) };

    const hal::pixel_point sz       = m_renderer.size();
    const hal::coord_t     fly_dist = sz.y / 4.0;

    // The "Made with" part.
    text made_with {
        .tex = this->load_pixel_art(font.render("Made with ")),
        .pos = hal::anchor::resolve(hal::anchor::center, hal::pixel_point(sz / 2.0), font.size_text(std::string { "Made with " } + words.front()))
    };

    text current {
        .tex = this->load_pixel_art(font.render(words.front())),
        .pos = { made_with.pos.x + made_with.tex.size().x, made_with.pos.y - fly_dist }
    };

    modifier<hal::texture, lyo::i16> fmod {
        &current.tex,
        lyo::cast<lyo::i16>(hal::color::max),
        hal::color::min,
        &bezier::ease_out
    };

    modifier<hal::coord_t, hal::coord_t> mmod {
        &current.pos.y,
        fly_dist,
        made_with.pos.y - fly_dist,
        &bezier::ease_out
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
                    fmod.func   = &bezier::ease_out;

                    mmod.object = &current.pos.x;
                    mmod.dist   = -made_with.tex.size().x;
                    mmod.base   = made_with.pos.x + made_with.tex.size().x;
                    mmod.func   = &bezier::quadratic;
                }

                else
                {
                    phase = mw_rotation_out;

                    mmod.base = made_with.pos.y;
                    mmod.func = &bezier::ease_in;

                    fmod.dist = -fmod.dist;
                    fmod.base = hal::color::max;
                    fmod.func = &bezier::ease_in;
                }

                timer -= fade_time;
            }
            break;

        case mw_rotation_out:
            if (timer() >= fade_time)
            {
                phase = mw_rotation_in;

                fmod.func = &bezier::ease_out;
                fmod.dist = -fmod.dist;
                fmod.base = hal::color::min;

                mmod.base = made_with.pos.y - fly_dist;
                mmod.func = &bezier::ease_out;

                ++index;
                current.tex.change(m_renderer, font.render(words[index], index == words.size() - 1 ? hal::color::cyan : hal::color::white));

                timer -= fade_time;
            }
            break;

        case halcyon_shift:
            if (timer() >= fade_time)
            {
                phase = by_lyorig;

                made_with.tex.change(m_renderer, font.render("by lyorig"));
                made_with.pos.y += current.tex.size().y - made_with.tex.size().y;

                mmod.object = &made_with.pos.x;
                mmod.base   = sz.x;
                mmod.dist   = -sz.x + current.pos.x + current.tex.size().x;
                mmod.func   = &bezier::ease_out;

                fmod.base = hal::color::max;
                fmod.dist = 0;
                fmod.func = &bezier::ease_out;

                timer -= fade_time;

                timer.reset();
            }
            break;

        case by_lyorig:
            if (timer() >= fade_time)
                goto Hell;
            break;
        }

        const lyo::f64 rel_time { timer() / fade_time };

        fmod.object->set_opacity(static_cast<hal::color::value>(fmod.get(rel_time)));
        *mmod.object = mmod.get(rel_time);

        draw(made_with);
        draw(current);
        HAL_DRAW_CONSOLE(m_renderer, m_font);
    }
Hell:;
}

void game::start()
{
    if (m_args.has("-nogame"))
        return;

    while (this->update())
    {
        this->dispatch_event(m_state->update(m_input, this->delta()));
        HAL_DRAW_CONSOLE(m_renderer, m_font);
    }
}

bool game::update()
{
    m_delta.reset();
    m_renderer.present();

    return m_input.update();
}

void game::dispatch_event(state::type tp)
{
    using enum state::type;

    if (tp == none) [[likely]]
        return;

    switch (tp)
    {
    case menu:
        m_state = &m_menuState;
        break;
    case playing:
        m_state = &m_playingState;
        break;
    case paused:
        m_state = &m_pausedState;

        break;
    case none:
        HAL_PANIC("Dispatching received \"none\" event");
    }
}

lyo::f64 game::delta() const
{
    return m_delta();
}

hal::texture game::load_pixel_art(const hal::surface& surf)
{
    return { m_renderer, surf };
}

hal::font game::load_appx_font(const char* path, lyo::f64 screen_height_percentage)
{
    // Desired height.
    const hal::pixel_t dh { lyo::cast<hal::pixel_t>(constants::render_height * (screen_height_percentage / 100.0)) };

    lyo::u8   pt { 0 };
    hal::font fnt { m_ttf.load(path, pt) };

    for (; fnt.size_text(" ").y < dh; fnt = m_ttf.load(path, ++pt))
    {
        HAL_ASSERT(pt != std::numeric_limits<lyo::u8>::max(), "Couldn't size font to desired height");
    }

    return fnt;
}

hal::font game::load_appx_font(const char* path, const char* example_text, lyo::f64 screen_width_percentage)
{
    // Desired width.
    const hal::pixel_t dw { lyo::cast<hal::pixel_t>(m_renderer.size().x * (screen_width_percentage / 100.0)) };

    lyo::u8   pt { 0 };
    hal::font fnt { m_ttf.load(path, pt) };

    for (; fnt.size_text(example_text).x < dw; fnt = m_ttf.load(path, ++pt))
    {
        HAL_ASSERT(pt != std::numeric_limits<lyo::u8>::max(), "Couldn't size font to desired width");
    }

    return fnt;
}

hal::pixel_point game::apx_size()
{
    using namespace constants;

    const hal::pixel_point dsz { m_video.display_at(m_window.display_index()).size() };
    const lyo::f64         aspect_ratio { lyo::f64(dsz.x) / dsz.y };

    return { lyo::cast<hal::pixel_t>(art_height * aspect_ratio), art_height };
}