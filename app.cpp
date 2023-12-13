#include "app.hpp"

#include "halcyon/input_handler.hpp"
#include "halcyon/window.hpp"

using namespace hq;

app::app(const lyo::parser& args, const char* window_name)
    : m_input {
        m_eng
    }
    , mixer { m_eng }
    , window { m_eng, window_name, hal::fullscreen_mode, { hal::renderer::accelerated, !args.has("-novsync") ? hal::renderer::vsync : hal::renderer::none } }
    , image { m_eng, { hal::image_loader::jpg, hal::image_loader::png } }
    , ttf { m_eng }
{
}

bool app::update()
{
    window.present();

    return m_input.update();
}

const hal::input_handler& app::input() const { return m_input; }