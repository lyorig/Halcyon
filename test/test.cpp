#include <halcyon/engine.hpp>
#include <halcyon/texture.hpp>
#include <halcyon/ttf_engine.hpp>
#include <halcyon/window.hpp>
#include <iostream>
#include <lyo/argparse.hpp>

constexpr int  draw_iters { 32 };
constexpr char string[] { "abcdef1234" };

struct holder
{
    LYO_MAYBE_EMPTY hal::engine eng;
    LYO_MAYBE_EMPTY hal::video vid { eng };
    LYO_MAYBE_EMPTY hal::ttf_engine ttf { eng };

    hal::window     wnd { vid, "Renderer proxy", {}, { 100, 100 }, { hal::window::hidden }, { hal::renderer::accelerated } };
    const hal::font fnt { ttf.load("assets/m5x7.ttf", 144) };
};

void surface_drawing(holder& hld)
{
    hal::surface res { hld.vid, { 1024, 768 } };

    for (int i = 0; i < draw_iters; ++i)
        hal::blit(hld.fnt.render(string))(res);

    hal::static_texture tes { hld.wnd, res };
}

void texture_drawing(holder& hld)
{
    hal::target_texture tex { hld.wnd, { 1024, 768 } };

    hld.wnd.renderer.set_target(tex);

    for (int i = 0; i < draw_iters; ++i)
    {
        const hal::static_texture dt { hld.wnd, hld.fnt.render(string) };
        hal::draw { dt }();
    }

    hld.wnd.set_as_target();
}

int main(int argc, char* argv[])
{
    using sz = std::size_t;
    const auto iter = lyo::parser { argc, argv }.parse<sz>("-iter=", 100);

    holder h;

    lyo::precise_timer tmr;

    for (sz i { 0 }; i < iter; ++i)
        surface_drawing(h);

    const lyo::f64 surface_result { tmr() };

    tmr.reset();

    for (sz i { 0 }; i < iter; ++i)
        texture_drawing(h);

    const lyo::f64 texture_result { tmr() };

    std::cout << "Surface drawing took " << surface_result << "s.\n";
    std::cout << "Texture drawing took " << texture_result << "s.\n";
    std::cout << "Texture drawing is " << surface_result / texture_result << "x faster at " << iter << " iterations.\n";

    return EXIT_SUCCESS;
}