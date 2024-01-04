#include <HalQ/app.hpp>

using namespace hq;

app::app(lyo::parser&& p, const char* window_name)
    : m_video { m_eng }
    , m_audio { m_eng }
    , input { m_eng }
    , mixer { m_audio, p.parse<lyo::u32>("-freq=", 44100), 16, hal::chunk::quality::medium }
    , window { m_video, window_name, {}, { 1280, 720 }, {} }
    , renderer { window, { hal::renderer::accelerated, !p.has("-xv") ? hal::renderer::vsync : hal::renderer::none } }
    , args { p }
    , image { m_video, { hal::image_loader::jpg, hal::image_loader::png } }
    , ttf { m_video }
{
}

bool app::update()
{
    m_delta.reset();
    renderer.present();

    return input.update();
}

lyo::f64 app::delta() const
{
    return m_delta();
}