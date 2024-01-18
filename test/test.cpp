#include <halcyon/halcyon.hpp>
#include <lyo/argparse.hpp>

using namespace hal::literals;

class game
{

    LYO_NOSIZE hal::engine eng;
    LYO_NOSIZE hal::video vid { eng };

    hal::input_handler inp { eng };

    const hal::pixel_t tile_size;

    hal::window wnd; // initialized in c-tor

public:
    hal::renderer rnd { wnd, { hal::renderer::accelerated, hal::renderer::vsync } };

    game(hal::pixel_t tile_size)
        : tile_size { tile_size }
        , wnd { vid, "Šachy", {}, { hal::pixel_t(tile_size * 10), hal::pixel_t(tile_size * 8) }, {} }
    {
        rnd.set_draw_color(hal::color::weezer_blue);
    }

    bool update()
    {
        rnd.present();
        return inp.update();
    }

    hal::target_texture compose()
    {
        const hal::pixel_t board_size = tile_size * 8;

        hal::target_texture tx { rnd, { board_size, board_size } };
        bool                which { false };

        constexpr hal::color cs[2] { 0x5e1f00, 0xf79e72 };

        {
            hal::target_lock tl { rnd, tx };
            hal::color_lock  cl { rnd, cs[0] };

            for (int x { 0 }; x < board_size; x += tile_size)
            {
                for (int y { 0 }; y < board_size; y += tile_size)
                {
                    cl.set(cs[which]);
                    rnd.fill_rect(hal::sdl::coord_rect(x, y, tile_size, tile_size));
                    which = !which;
                }

                which = !which;
            }
        }

        return tx;
    }
};

int main(int argc, char* argv[])
{
    game g { lyo::parser { argc, argv }.parse<hal::pixel_t>("-size=", 64) };

    const hal::target_texture tx { g.compose() };

    while (g.update())
    {
        hal::draw { tx }(g.rnd);
    }
}