#include <halcyon/halcyon.hpp>
#include <lyo/argparse.hpp>

using namespace hal::literals;

// this is goofy asf
enum piece
{
    vez,
    kun,
    strelec,
    kralovna,
    kral,
    pesec,
    MAX
};

hal::pixel_rect get_src(bool team, piece type)
{
    constexpr std::array<hal::pixel_rect, MAX * 2> sources
    {
        {
            { 0, 4, 126, 150 },
            { 167, 2, 157, 154 },
            { 351, 4, 150, 150 },
            { 523, 0, 169, 157 },
            { 710, 0, 158, 156 },
            { 917, 5, 107, 150 },
            { 0, 4, 126, 150 },
            { 167, 159, 157, 154 },
            { 351, 161, 150, 150 },
            { 523, 157, 169, 157 },
            { 710, 157, 158, 156 },
            { 917, 162, 107, 150 }
        }
    };

    return sources[type + team * MAX];
}

class game
{

    LYO_NOSIZE hal::engine eng;
    LYO_NOSIZE hal::video vid { eng };

    hal::input_handler inp { eng };

    const hal::pixel_t tile_size;

    hal::window wnd; // initialized in c-tor

public:
    hal::renderer rnd { wnd, { hal::renderer::accelerated, hal::renderer::vsync } };

    LYO_NOSIZE hal::image_loader img {
        vid, {hal::image_loader::png}
    };

    game(hal::pixel_t tile_size)
        : tile_size { tile_size }
        , wnd { vid, "Chess. Made with Halcyon.", { hal::pixel_t(tile_size * 10), hal::pixel_t(tile_size * 8) }, {} }
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

    hal::pixel_point tile_at(lyo::u8 x, lyo::u8 y) const
    {
        return {
            lyo::cast<hal::pixel_t>(x * tile_size),
            lyo::cast<hal::pixel_t>(y * tile_size)
        };
    }
};

constexpr piece start_pos[32] { // I am not going to heaven
    vez,    kun,    strelec,    kral,       kralovna,   strelec,    kun,    vez,
    pesec,  pesec,  pesec,      pesec,      pesec,      pesec,      pesec,  pesec,

    pesec,  pesec,  pesec,      pesec,      pesec,      pesec,      pesec,  pesec,
    vez,    kun,    strelec,    kralovna,   kral,       strelec,    kun,    vez
};

int main(int argc, char* argv[])
{
    game g { lyo::parser { argc, argv }.parse<hal::pixel_t>("-size=", 64) };

    const hal::target_texture tx { g.compose() };
    const hal::texture        tex { g.rnd, g.img.load("assets/pieces.png") };

    while (g.update())
    {
        hal::draw { tx }(g.rnd);
    }

    return EXIT_SUCCESS;
}