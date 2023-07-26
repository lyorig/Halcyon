#include "scene.hpp"

#include "ents/entity.hpp"

using namespace game;

scene::scene(hal::engine& eng, const char* game_title) :
    m_input { eng },
    m_window { eng, game_title, hal::fullscreen, hal::renderer::accelerated | hal::renderer::vsync },
    m_iLoader { m_window, hal::image_loader::png },
    m_fLoader { m_window }
{
    m_level.spawn<d1>(hal::coordinate {});
    m_level.spawn<d2>(hal::coordinate {});
}

bool scene::update() noexcept
{
    m_input.update();

    if (m_input.should_quit())
        return false;

    m_level.update(m_delta());
    m_window.present();

    m_delta.reset();

    return true;
}

void scene::load_level(const char* path) noexcept
{
}