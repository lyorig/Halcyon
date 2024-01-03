#include <HalQ/manager.hpp>
#include <halcyon/halcyon.hpp>
#include <iostream>
#include <lyo/argparse.hpp>
#include <vector>

using sz = std::size_t;

constexpr int  draw_iters { 32 };
constexpr char string[] { "abcdef1234" };
constexpr char help_text[] {
    "Haltest, by lyorig.\n"
    "Options:\n"
    "\t-h\t- Show this message.\n"
    "\t-s\t- Measure surface performance.\n"
    "\t-t\t- Measure texture performance.\n"
    "\t-ecs\t- Test the ECS.\n"
    "\t-iter=[num]\t- The amount of iterations (default 100).\n"
};

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

void ecs_test(sz iters)
{
    hq::manager mgr;

    std::vector<hq::entity::ID> ids;

    for (sz i { 0 }; i < iters; ++i)
        ids.push_back(mgr.add<hq::velocity, hq::position>());
}

int main(int argc, char* argv[])
{
    lyo::parser p { argc, argv };

    const auto iter = p.parse<sz>("-iter=", 100);

    if (p.has("-h"))
    {
        std::cout << help_text << 'n';
        return EXIT_SUCCESS;
    }

    const bool arg_t { p.has("-t") }, arg_s { p.has("-s") }, arg_ecs { p.has("-ecs") };

    if (!(arg_t || arg_s || arg_ecs))
    {
        std::cout << "No measuring options specified. Exiting.\n";
        return EXIT_FAILURE;
    }

    if (arg_ecs)
        ecs_test(iter);

    if (arg_t || arg_s)
    {
        holder   h;
        lyo::f64 surface_result, texture_result;

        lyo::precise_timer tmr;

        if (arg_s)
        {
            for (sz i { 0 }; i < iter; ++i)
                surface_drawing(h);

            surface_result = tmr();
        }

        tmr.reset();

        if (arg_t)
        {
            for (sz i { 0 }; i < iter; ++i)
                texture_drawing(h);

            texture_result = tmr();
        }

        if (arg_s)
            std::cout << "Surface drawing took " << surface_result << "s\n";

        if (arg_t)
            std::cout << "Texture drawing took " << texture_result << "s\n";

        if (arg_t && arg_s)
            std::cout << "Result: Texture drawing is " << surface_result / texture_result << "x faster\n";
    }

    return EXIT_SUCCESS;
}