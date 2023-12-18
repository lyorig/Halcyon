#include "app.hpp"

#include "halcyon/input_handler.hpp"
#include "halcyon/window.hpp"

using namespace hq;

app::app(lyo::parser&& p, const char* window_name)
    : m_input { m_eng }
    , mixer { m_eng }
    , window { m_eng, window_name, hal::fullscreen_mode, { hal::renderer::accelerated, !p.has("-xv") ? hal::renderer::vsync : hal::renderer::none } }
    , args { p }
    , image { m_eng, { hal::image_loader::jpg, hal::image_loader::png } }
    , ttf { m_eng }
{
}

bool app::update()
{
    m_delta.reset();
    window.present();

    return m_input.update();
}

const hal::input_handler& app::input() const { return m_input; }

lyo::f64 app::delta() const
{
    return m_delta();
}