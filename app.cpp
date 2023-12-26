#include "app.hpp"

#include "halcyon/input_handler.hpp"
#include "halcyon/window.hpp"

using namespace hq;

app::app(lyo::parser&& p, const char* window_name)
    : input { m_eng }
    , mixer { m_audio, p.parse<lyo::u32>("-freq=", 44100), 16, hal::chunk::quality::medium }
    , window { m_video, window_name, hal::fullscreen_mode, { hal::renderer::accelerated, !p.has("-xv") ? hal::renderer::vsync : hal::renderer::none } }
    , args { p }
    , image { m_eng, { hal::image_loader::jpg, hal::image_loader::png } }
    , ttf { m_eng }
{
}

bool app::update()
{
    m_delta.reset();
    window.present();

    return input.update();
}

lyo::f64 app::delta() const
{
    return m_delta();
}