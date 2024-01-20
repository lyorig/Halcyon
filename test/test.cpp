// #define HAL_INTEGRAL_COORD

#include <halcyon/halcyon.hpp>
#include <lyo/argparse.hpp>
#include <vector>

class chess
{
    struct piece
    {
        using pos_t = lyo::i8;
        using pos   = hal::point<pos_t>;

        enum type : lyo::u8
        {
            rook,
            knight,
            bishop,
            queen,
            king,
            pawn,
            amount,
            invalid
        };

        enum team : bool
        {
            white,
            black
        };

        constexpr piece()
            : tp { invalid } {

            };

        constexpr piece(const piece&) = default;

        constexpr piece(type t1, team t2)
            : tp { t1 }
            , tm { t2 }
        {
        }

        constexpr piece(piece&& mv)
            : tp { mv.tp }
            , tm { mv.tm }
        {
            if (this == &mv)
                return;

            mv.tp = invalid;
        }

        constexpr piece& operator=(const piece&) = default;

        constexpr void operator=(piece&& mv)
        {
            if (this == &mv)
                return;

            tp = mv.tp;
            tm = mv.tm;

            mv.tp = invalid;
        };

        consteval static lyo::f64 szmul()
        {
            return 0.8;
        }

        consteval static pos_t invalid_pos()
        {
            return 16;
        }

        constexpr bool valid() const
        {
            return tp != invalid;
        }

        type tp;
        team tm;
    };

public:
    chess(hal::pixel_t tile_size)
        : m_video { m_engine }
        , m_image { m_video, { hal::image_loader::png } }
        , m_input { m_engine }
        , m_tileSize { tile_size }
        , m_tilePoint { m_tileSize, m_tileSize }
        , m_wnd { m_video, "Chess. Made with Halcyon.", { hal::pixel_t(tile_size * 10), hal::pixel_t(tile_size * 8) }, {} }
        , m_rnd { m_wnd, { hal::renderer::accelerated, hal::renderer::vsync } }
        , m_board { m_rnd, this->compose() }
        , m_pieces { m_rnd, m_image.load("assets/pieces.png") }
        , m_nowChosen { piece::invalid_pos() }
        , m_turn { piece::white }
        , m_isMoving { false }
    {
        this->reset_pieces();
        m_rnd.set_draw_color(hal::color::weezer_blue);
    }

    bool update()
    {
        hal::draw { m_board }(m_rnd);

        this->process_click();
        this->draw_pieces();

        m_rnd.present();
        return m_input.update();
    }

private:
    hal::surface compose()
    {
        const hal::pixel_t board_size = m_tileSize * 8;

        hal::surface canvas { m_video, { board_size, board_size } };
        bool         which { false };

        constexpr hal::color cs[2] { 0x5e1f00, 0xf79e72 };

        for (int x { 0 }; x < board_size; x += m_tileSize)
        {
            for (int y { 0 }; y < board_size; y += m_tileSize)
            {
                canvas.fill_rect(hal::sdl::coord_rect(x, y, m_tileSize, m_tileSize), cs[which]);
                which = !which;
            }

            which = !which;
        }

        return canvas;
    }

    // Get the tiles to which a piece can move.
    constexpr void set_moveables(piece::pos p, piece pc)
    {
        const auto add = [this](piece::pos p) -> void
        {
            m_canMoveTo.emplace_back(p.x * m_tileSize, p.y * m_tileSize, m_tileSize, m_tileSize);
        };

        add(p);

        switch (pc.tp)
        {
            using enum piece::type;

        case rook:
        {
            constexpr piece::pos dir[] { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
            for (auto mod : dir)
            {
                for (auto curr = p + mod; vloc(curr, pc.tm); curr += mod)
                {
                    HAL_PRINT("Found valid mvmt @ ", curr);
                    add(curr);
                }
            }
            break;
        }
        case knight:
        {
            constexpr piece::pos dir[] { { 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 }, { 1, 2 }, { -1, 2 }, { 1, -2 }, { -1, -2 } };
            for (auto mod : dir)
            {
                if (auto curr = p + mod; vloc(mod, pc.tm))
                    add(curr);
            }
            break;
        }
        case bishop:
        {
            constexpr piece::pos dir[] { { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };
            for (auto mod : dir)
            {
                for (auto curr = p + mod; vloc(curr, pc.tm); curr += mod)
                    add(curr);
            }
            break;
        }
        case queen:
        {
            constexpr piece::pos dir[] { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };
            for (auto mod : dir)
            {
                p += mod;
                for (auto curr = p + mod; vloc(curr, pc.tm); curr += mod)
                    add(curr);
            }
            break;
        }
        case king:
        {
            constexpr piece::pos dir[] { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
            for (auto mod : dir)
            {
                if (auto curr = p + mod; vloc(curr, pc.tm))
                    add(curr);
            }
            break;
        }
        case pawn: // Extra checks are necessary here...
        {
            // Attack directions
            constexpr piece::pos_t adir[] { -1, 2 };

            piece::pos_t steps { 1 };
            piece::pos_t ydir;

            if (pc.tm == piece::white) // can only go down
            {
                steps += p.y == 1;
                ydir = -1;
            }

            else // can only go up
            {
                steps += p.y == 6;
                ydir = 1;
            }

            // Check the standard movement path
            {
                piece::pos curr = p;
                for (curr.y += ydir; vloc_pawn(curr) && steps != 0; --steps)
                {
                    add(curr);
                }
            }

            p.y += ydir; // p can be "thrown away" at this point
            for (auto dir : adir)
            {
                p.x += dir;
                if (vloc(p, pc.tm))
                    add(p);
            }

            break;
        }
        default:
            std::unreachable();
        }
    }

    void process_click()
    {
        const hal::pixel_point pt { m_input.mouse() };

        if (m_input.pressed(hal::button::left_mouse) && pt < m_board.size())
        {
            HAL_PRINT("Got click at ", m_input.mouse());

            if (m_isMoving)
            {
                assert(m_nowChosen.x != piece::invalid_pos());

                for (const auto pos : m_canMoveTo)
                {
                    if (hal::sdl::coord_point(pt) | pos)
                    {
                        m_isMoving = false;

                        piece::pos npos = pos.pos / m_tilePoint;
                        piece_at(npos)  = std::move(piece_at(m_nowChosen));

                        m_nowChosen.x = piece::invalid_pos();
                        m_canMoveTo.clear();

                        HAL_PRINT("Moving to ", npos);

                        break;
                    }
                }

                // Switch the playing team.
                m_turn = piece::team(!m_turn);
            }

            else // Not moving.
            {
                assert(m_nowChosen.x == piece::invalid_pos());

                const piece::pos chosen = pt / m_tilePoint;

                m_nowChosen     = chosen;
                const piece& pc = piece_at(chosen);

                HAL_PRINT("Selected piece at ", chosen, ", type ", hal::to_printable_int(pc.tp), ", team ", pc.tm);

                if (pc.valid())
                {
                    m_isMoving = true;

                    set_moveables(chosen, pc);
                }
            }
        }
    }

    constexpr void reset_pieces()
    {
        using pa   = std::array<piece::pos, 8>;
        using pair = std::pair<lyo::u8, pa>;

        const auto push_list = [&](const std::initializer_list<pair>& pr, piece::team tm)
        {
            for (lyo::u8 t { piece::rook }; t != piece::amount; ++t)
            {
                const pair& curr { *(pr.begin() + t) };

                for (lyo::u8 i { 0 }; i < curr.first; ++i)
                {
                    piece_at(curr.second[i]) = { piece::type(t), tm };
                }
            }
        };

        const std::initializer_list<pair> pos_white {
            { 2, { { { 0, 0 }, { 7, 0 } } } },                                                            // rook
            { 2, { { { 1, 0 }, { 6, 0 } } } },                                                            // knight
            { 2, { { { 2, 0 }, { 5, 0 } } } },                                                            // bishop
            { 1, { { { 3, 0 } } } },                                                                      // queen
            { 1, { { { 4, 0 } } } },                                                                      // king
            { 8, { { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 }, { 4, 1 }, { 5, 1 }, { 6, 1 }, { 7, 1 } } } } // pawn
        };

        const std::initializer_list<pair> pos_black {
            { 2, { { { 0, 7 }, { 7, 7 } } } },                                                            // rook
            { 2, { { { 1, 7 }, { 6, 7 } } } },                                                            // knight
            { 2, { { { 2, 7 }, { 5, 7 } } } },                                                            // bishop
            { 1, { { { 4, 7 } } } },                                                                      // queen
            { 1, { { { 3, 7 } } } },                                                                      // king
            { 8, { { { 0, 6 }, { 1, 6 }, { 2, 6 }, { 3, 6 }, { 4, 6 }, { 5, 6 }, { 6, 6 }, { 7, 6 } } } } // pawn
        };

        push_list(pos_black, piece::black);
        push_list(pos_white, piece::white);
    }

    void draw_pieces()
    {
        for (lyo::u8 i { 0 }; i < m_boardState.size(); ++i)
        {
            const piece& pc { m_boardState[i] };

            if (pc.valid())
            {
                const std::div_t d { std::div(i, 8) };

                // initialized via size
                const hal::pixel_point sz         = m_tilePoint * piece::szmul();
                const hal::pixel_point tilecenter = get_dst({ piece::pos_t(d.rem), piece::pos_t(d.quot) }) + m_tilePoint / 2;
                const hal::pixel_point final      = hal::anchor::resolve(hal::anchor::center, tilecenter, sz);

                hal::draw { m_pieces }
                    .from(get_src(pc.tm, pc.tp))
                    .to({ final, sz })(m_rnd);
            }
        }

        hal::color_lock cl { m_rnd, { 0x00FF00, 128 } };

        if (m_isMoving)
            m_rnd.fill_rects(m_canMoveTo);
    }

    constexpr hal::pixel_point tile_at(piece::pos p) const
    {
        return {
            lyo::cast<hal::pixel_t>(p.x * m_tileSize),
            lyo::cast<hal::pixel_t>(p.y * m_tileSize)
        };
    }

    constexpr static hal::pixel_rect get_src(piece::team tm, piece::type tp)
    {
        return { hal::pixel_t(tp * 32), hal::pixel_t(tm * 32), 32, 32 };
    }

    constexpr hal::pixel_point get_dst(piece::pos p) const
    {
        return { hal::pixel_t(m_tileSize * p.x), hal::pixel_t(m_tileSize * p.y) };
    }

    constexpr piece& piece_at(piece::pos pos)
    {
        return m_boardState[pos.y * 8 + pos.x];
    }

    constexpr const piece& piece_at(piece::pos pos) const
    {
        return m_boardState[pos.y * 8 + pos.x];
    }

    // Valid location.
    constexpr bool vloc(piece::pos p, piece::team tm) const
    {
        const piece& pc { piece_at(p) };
        return vloc_checkpos(p) && (!pc.valid() || pc.tm != tm);
    }

    // Valid location for a pawn's frontal move.
    constexpr bool vloc_pawn(piece::pos p) const
    {
        return vloc_checkpos(p) && !piece_at(p).valid();
    }

    constexpr bool vloc_checkpos(piece::pos p) const
    {
        return p.x >= 0 && p.x && p.x < 8 && p.y >= 0 && p.y < 8;
    }

    std::array<piece, 64>             m_boardState;
    std::vector<hal::sdl::coord_rect> m_canMoveTo;

    LYO_NOSIZE hal::engine m_engine;
    LYO_NOSIZE hal::video m_video;
    LYO_NOSIZE hal::image_loader m_image;

    hal::input_handler m_input;

    const hal::pixel_t     m_tileSize;
    const hal::pixel_point m_tilePoint;

    hal::window   m_wnd;
    hal::renderer m_rnd;

    const hal::texture m_board;
    const hal::texture m_pieces;

    piece::pos m_nowChosen;

    piece::team m_turn;
    bool        m_isMoving;
};

int main(int argc, char* argv[])
{
    chess g { lyo::parser { argc, argv }.parse<hal::pixel_t>("-size=", 64) };

    while (g.update())
        ;

    return EXIT_SUCCESS;
}