#include <halcyon/halcyon.hpp>
#include <iostream>
#include <lyo/argparse.hpp>

constexpr int  draw_iters { 32 };
constexpr char string[] { "abcdef1234" };

struct holder
{
    LYO_MAYBE_EMPTY hal::engine eng;
    LYO_MAYBE_EMPTY hal::video vid { eng };
    LYO_MAYBE_EMPTY hal::ttf_engine ttf { vid };

    hal::window   wnd { vid, "Renderer proxy", {}, { 100, 100 }, { hal::window::hidden } };
    hal::renderer rnd { wnd, { hal::renderer::accelerated } };

    const hal::font fnt { ttf.load("assets/m5x7.ttf", 144) };
};

void surface_drawing(holder& hld)
{
    hal::surface res { hld.vid, { 1024, 768 } };

    for (int i = 0; i < draw_iters; ++i)
        hal::blit(hld.fnt.render(string))(res);

    hal::texture tes { hld.rnd, res };
}

void texture_drawing(holder& hld)
{
    hal::target_texture tex { hld.rnd, { 1024, 768 } };
    hal::target_lock    tl { hld.rnd, tex };

    for (int i = 0; i < draw_iters; ++i)
    {
        const hal::texture dt { hld.rnd, hld.fnt.render(string) };
        hal::draw { dt }(hld.rnd);
    }
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

    std::cout << "Surface drawing took " << surface_result << "s\n";
    std::cout << "Texture drawing took " << texture_result << "s => " << surface_result / texture_result << "x faster at " << iter << " iterations.\n";

    return EXIT_SUCCESS;
}