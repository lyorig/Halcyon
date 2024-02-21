#include <halcyon/halcyon.hpp>
#include <thread>

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    hal::cleanup    _;
    hal::ttf_engine __;

    hal::window   w { "Halcyon", { 640, 480 }, {} };
    hal::renderer r { w, { hal::renderer::accelerated } };

    hal::input_handler i;

    {
        const hal::texture t { r, hal::font { argc < 2 ? "assets/m5x7.ttf" : argv[1], 144 }.render(argc < 3 ? "Made with Halcyon" : argv[2], hal::color::cyan) };

        w.set_size(t.size());

        r.draw(t)();
        r.present();
    }

    while (i.update())
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(100ms);
    }
}