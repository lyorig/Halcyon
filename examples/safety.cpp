#include <halcyon/video.hpp>

int main(int, char*[])
{
    // static_assert(hal::meta::is_correct_main<main>);

    // Co funguje:
    hal::context ctx;

    hal::system::video vid { ctx };
    hal::window        wnd = vid.make_window("Okno", { 640, 480 });
    hal::renderer      rnd { wnd.make_renderer() };

    // Co nefunguje:
    // hal::system::video vidx { hal::context {} };
    // hal::window        wndx { hal::system::video { ctx }.make_window("Okno", { 640, 480 }) };
    // hal::renderer      rndx { hal::window {}.make_renderer() };

    return EXIT_SUCCESS;
}