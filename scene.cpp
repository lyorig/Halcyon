#include "scene.hpp"

#include <lyo/slider.hpp>

#include "ents/entity.hpp"

using namespace game;

scene::scene(hal::engine& eng, lyo::u32 freq) :
    m_input { eng },
    m_mixer { eng, freq, 16, hal::chunk_quality::high },
    m_window { eng, "HalodaQuest", hal::fullscreen, hal::renderer::accelerated | hal::renderer::vsync },
    m_iLoader { m_window, hal::image_loader::png },
    m_ttfEngine { m_window },
    m_font { m_ttfEngine.load_font("assets/fonts/m5x7.ttf", 42) }
{
    this->intro();
}

void scene::main_loop() noexcept
{
    while (this->update())
        ;
}

bool scene::update() noexcept
{
    m_input.update();

    // HALFIX: Edit this after a proper pause menu is implemented.
    if (m_input.should_quit() || m_input.pressed(hal::button::esc))
        return false;

    m_level.update(m_delta());

    HAL_CONSOLE_DRAW(m_font, m_window);

    m_window.present();

    m_delta.reset();

    return true;
}

void scene::intro() noexcept
{
    hal::texture canvas {
        m_window, {640, 480}
    };

    const hal::renderer& rend { m_window.renderer };

    rend.set_target(canvas);

    {
        hal::texture top_text { m_window, m_font.render("Made with Halcyon") };
        // We live in a society
        hal::texture bottom_text { m_window, m_font.render("by lyorig", 0x00FFFF) };

        top_text.draw(hal::anchor::top_left, top_text.vw(100.0));
        bottom_text.draw(hal::anchor::bottom_right, bottom_text.vw(50.0));
    }

    rend.reset_target();

    m_mixer.mus.play("assets/ost/intro.mp3");

    while (!m_input.should_quit() && !m_input.pressed(hal::button::esc) && m_mixer.mus.playing())
    {
        canvas.draw(hal::anchor::center);

        m_window.present();

        m_input.update();
    }

    m_mixer.mus.pause();
}