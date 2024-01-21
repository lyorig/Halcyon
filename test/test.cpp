#include "piece.hpp"
#include <halcyon/halcyon.hpp>
#include <lyo/argparse.hpp>
#include <vector>

class chess
{
public:
    chess(hal::pixel_t tile_size)
        : m_video { m_engine }
        , m_image { m_video, { hal::image_loader::png } }
        , m_input { m_engine }
        , m_tileSize { tile_size }
        , m_tilePoint { m_tileSize, m_tileSize }
        , m_wnd { m_video, "HalChess", { hal::pixel_t(tile_size * 10), hal::pixel_t(tile_size * 8) }, {} }
        , m_rnd { m_wnd, { hal::renderer::accelerated, hal::renderer::vsync } }
        , m_board { m_rnd, this->compose_board() }
        , m_pieces { m_rnd, m_image.load("assets/pieces.png") }
        , m_nowChosen { piece::invalid_pos() }
        , m_whoseTurn { piece::white }
        , m_isMoving { false }
    {
        reset_pieces();
        set_team(piece::white);
    }

    bool update()
    {
        hal::draw { m_board }(m_rnd);

        process_click();
        draw_pieces();

        m_rnd.present();
        return m_input.update();
    }

private:
    void draw_pieces()
    {
        for (lyo::u8 i { 0 }; i < m_boardState.size(); ++i)
        {
            const piece& pc { m_boardState[i] };

            if (pc.valid())
            {
                const std::div_t d { std::div(i, 8) };

                const hal::pixel_point sz         = m_tilePoint * piece::szmul();
                const hal::pixel_point tilecenter = get_dst({ piece::pos_t(d.rem), piece::pos_t(d.quot) }) + m_tilePoint / 2;
                const hal::pixel_point final      = hal::anchor::resolve(hal::anchor::center, tilecenter, sz);

                hal::draw { m_pieces }
                    .from(get_src(pc.tm, pc.tp))
                    .to({ final, sz })(m_rnd);
            }
        }

        hal::color_lock cl { m_rnd, { 0x00FF00, 100 } };

        if (m_isMoving)
            m_rnd.fill_rects(m_canMoveTo);
    }

    void process_click()
    {
        const hal::pixel_point pt { m_input.mouse() };

        if (m_input.pressed(hal::button::right_mouse) && m_isMoving)
        {
            m_canMoveTo.clear();
            m_nowChosen.x = piece::invalid_pos();
            m_isMoving    = false;
        }

        if (m_input.pressed(hal::button::left_mouse) && pt < m_board.size())
        {
            HAL_PRINT("Got click at ", m_input.mouse());

            if (m_isMoving)
            {
                const piece::pos npos = pt / m_tilePoint;
                piece&           here { piece_at(npos) };

                if (npos == m_nowChosen)
                    goto Clear;

                // Have we chosen any valid position?
                for (const auto& pos : m_canMoveTo)
                {
                    if (hal::sdl::coord_point(pt) | pos)
                    {
                        if (npos != m_nowChosen)
                        {
                            if (takeable(here, m_whoseTurn))
                            {
                                m_dead.push_back(std::move(here));
                            }

                            here = std::move(piece_at(m_nowChosen));

                            HAL_PRINT("Moving to ", npos);

                            set_team(piece::team(!m_whoseTurn));
                        }

                        goto Clear;
                    }
                }

                // No valid position clicked on. Are we selecting another piece?
                if (friendly(here, m_whoseTurn))
                {
                    m_nowChosen = npos;
                    set_moveables(npos, here);
                }

                // Nope. Stop moving.
                else
                {
                Clear:
                    m_canMoveTo.clear();
                    m_nowChosen.x = piece::invalid_pos();
                    m_isMoving    = false;
                }
            }

            else
            {
                assert(m_nowChosen.x == piece::invalid_pos());

                const piece::pos chosen = pt / m_tilePoint;
                const piece&     pc     = piece_at(chosen);

                if (friendly(pc, m_whoseTurn))
                {
                    m_isMoving  = true;
                    m_nowChosen = chosen;

                    HAL_PRINT("Selected piece at ", chosen, ", type ", hal::to_printable_int(pc.tp), ", team ", pc.tm);

                    set_moveables(chosen, pc);
                }
            }
        }
    }

    // Get the tiles to which a piece can move.
    constexpr void set_moveables(piece::pos p, piece pc)
    {
        const auto add = [this](piece::pos p) -> void
        {
            m_canMoveTo.emplace_back(hal::sdl::coord_t(p.x * m_tileSize), hal::sdl::coord_t(p.y * m_tileSize), hal::sdl::coord_t(m_tileSize), hal::sdl::coord_t(m_tileSize));
        };

        m_canMoveTo.clear();

        add(p);

        switch (pc.tp)
        {
            using enum piece::type;

        case rook:
        {
            constexpr piece::pos dir[] { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
            for (auto mod : dir)
            {
                for (auto curr = p + mod; vloc(curr); curr += mod)
                {
                    const piece here { piece_at(curr) };

                    if (friendly(here, pc.tm))
                        break;

                    add(curr);

                    if (takeable(here, pc.tm))
                        break;
                }
            }
            break;
        }
        case knight:
        {
            constexpr piece::pos dir[] { { 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 }, { 1, 2 }, { -1, 2 }, { 1, -2 }, { -1, -2 } };
            for (auto mod : dir)
            {
                if (auto curr = p + mod; vloc(curr) && !friendly(piece_at(curr), pc.tm))
                {
                    add(curr);
                }
            }
            break;
        }
        case bishop:
        {
            constexpr piece::pos dir[] { { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };
            for (auto mod : dir)
            {
                for (auto curr = p + mod; vloc(curr); curr += mod)
                {
                    const piece here { piece_at(curr) };

                    if (friendly(here, pc.tm))
                        break;

                    add(curr);

                    if (takeable(here, pc.tm))
                        break;
                }
            }
            break;
        }
        case queen:
        {
            constexpr piece::pos dir[] { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };
            for (auto mod : dir)
            {
                for (auto curr = p + mod; vloc(curr); curr += mod)
                {
                    const piece here { piece_at(curr) };

                    if (friendly(here, pc.tm))
                        break;

                    add(curr);

                    if (takeable(here, pc.tm))
                        break;
                }
            }
            break;
        }
        case king:
        {
            constexpr piece::pos dir[] { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };
            for (auto mod : dir)
            {
                if (auto curr = p + mod; vloc(curr) && !friendly(piece_at(curr), pc.tm))
                    add(curr);
            }
            break;
        }
        case pawn: // Extra checks are necessary here...
        {
            // Attack directions
            constexpr piece::pos_t atkdir[] { -1, 2 };

            piece::pos_t steps { 1 };
            piece::pos_t ydir;

            if (pc.tm == piece::white) // can only go down
            {
                steps += p.y == 6;
                ydir = -1;
            }

            else // can only go up
            {
                steps += p.y == 1;
                ydir = 1;
            }

            // Check the standard movement path
            {
                piece::pos curr = p;
                for (curr.y += ydir; vloc(curr) && !takeable(piece_at(curr), pc.tm) && steps != 0; --steps, curr.y += ydir)
                {
                    add(curr);
                }
            }

            p.y += ydir; // p can be "thrown away" at this point
            for (auto dir : atkdir)
            {
                p.x += dir;
                if (vloc(p) && takeable(piece_at(p), pc.tm))
                    add(p);
            }

            break;
        }
        default:
            std::unreachable();
        }
    }

    void set_team(piece::team tm)
    {
        m_whoseTurn = tm;
        m_rnd.set_draw_color(0xFFFFFF * !tm);
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
        using namespace hal::literals;
        return { hal::pixel_t(tp * 32), hal::pixel_t(tm * 32), 32_px, 32_px };
    }

    constexpr hal::pixel_point get_dst(piece::pos p) const
    {
        return { hal::pixel_t(m_tileSize * p.x), hal::pixel_t(m_tileSize * p.y) };
    }

    constexpr piece& piece_at(piece::pos pos)
    {
        return m_boardState[std::size_t(pos.y) * 8 + pos.x];
    }

    constexpr const piece& piece_at(piece::pos pos) const
    {
        return m_boardState[std::size_t(pos.y) * 8 + pos.x];
    }

    // Valid location for a pawn's frontal move.
    constexpr bool vloc_pawn(piece::pos p) const
    {
        return vloc(p) && !piece_at(p).valid();
    }

    constexpr bool vloc(piece::pos p) const
    {
        return p.x >= 0 && p.x < 8 && p.y >= 0 && p.y < 8;
    }

    constexpr bool friendly(piece pc, piece::team tm) const
    {
        return pc.valid() && (pc.tm == tm || pc.tp == piece::king);
    }

    constexpr bool takeable(piece pc, piece::team tm) const
    {
        return pc.valid() && pc.tm != tm;
    }

    hal::surface compose_board()
    {
        hal::surface canvas { m_video, { hal::pixel_t(m_tileSize * 8), hal::pixel_t(m_tileSize * 8) } };

        std::array<hal::sdl::coord_rect, 32> m_rects;

        lyo::u8 idx { 0 };

        for (lyo::u8 y { 0 }; y < 8; ++y)
        {
            for (lyo::u8 x = y % 2; x < 8; x += 2, ++idx)
            {
                m_rects[idx] = { x * m_tileSize, y * m_tileSize, m_tileSize, m_tileSize };
            }
        }

        canvas.fill_rects(m_rects, 0x5e1f00);
        idx = 0;

        for (lyo::u8 y { 0 }; y < 8; ++y)
        {
            for (lyo::u8 x { !(y % 2) }; x < 8; x += 2, ++idx)
            {
                m_rects[idx] = { x * m_tileSize, y * m_tileSize, m_tileSize, m_tileSize };
            }
        }

        canvas.fill_rects(m_rects, 0xf79e72);

        return canvas;
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

        const std::initializer_list<pair> black {
            { 2, { { { 0, 0 }, { 7, 0 } } } },                                                            // rook
            { 2, { { { 1, 0 }, { 6, 0 } } } },                                                            // knight
            { 2, { { { 2, 0 }, { 5, 0 } } } },                                                            // bishop
            { 1, { { { 3, 0 } } } },                                                                      // queen
            { 1, { { { 4, 0 } } } },                                                                      // king
            { 8, { { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 }, { 4, 1 }, { 5, 1 }, { 6, 1 }, { 7, 1 } } } } // pawn
        };

        const std::initializer_list<pair> white {
            { 2, { { { 0, 7 }, { 7, 7 } } } },                                                            // rook
            { 2, { { { 1, 7 }, { 6, 7 } } } },                                                            // knight
            { 2, { { { 2, 7 }, { 5, 7 } } } },                                                            // bishop
            { 1, { { { 4, 7 } } } },                                                                      // queen
            { 1, { { { 3, 7 } } } },                                                                      // king
            { 8, { { { 0, 6 }, { 1, 6 }, { 2, 6 }, { 3, 6 }, { 4, 6 }, { 5, 6 }, { 6, 6 }, { 7, 6 } } } } // pawn
        };

        push_list(black, piece::black);
        push_list(white, piece::white);
    }

    std::array<piece, 64>         m_boardState;
    lyo::static_vector<piece, 31> m_dead;

    lyo::static_vector<hal::sdl::coord_rect, 28> m_canMoveTo;

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

    piece::team m_whoseTurn;
    bool        m_isMoving;
};

int main(int argc, char* argv[])
{
    chess g { lyo::parser { argc, argv }.parse<hal::pixel_t>("-size=", 64) };

    while (g.update())
        ;

    return EXIT_SUCCESS;
}