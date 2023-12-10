#include "mono_app.hpp"

#include "halcyon/input_handler.hpp"
#include "halcyon/window.hpp"

using namespace hq;

mono_app::mono_app(const char* window_name)
    : m_input {
        m_eng
    }
    , mixer { m_eng }
    , window { m_eng, window_name, hal::fullscreen_mode, { hal::renderer::accelerated } }
    , image { m_eng, { hal::image_loader::jpg, hal::image_loader::png } }
    , ttf { m_eng }
{
}

bool mono_app::update()
{
    window.present();

    return m_input.update();
}

const hal::input_handler& mono_app::input() const { return m_input; }