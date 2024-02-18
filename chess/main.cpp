#include "moves.hpp"

#include <halcyon/halcyon.hpp>
#include <thread>

HAL_DEBUG(
    std::ostream& operator<<(std::ostream& str, piece::pos p) {
        using pt = hal::point<piece::pos_t>;
        return str << pt { p.x, p.y };
    })

namespace cnst
{
    constexpr preprocessed_moves moves;

    // How many milliseconds to wait until pumping events. Should provide a balance
    // between CPU usage and responsiveness
    constexpr std::chrono::duration<std::int32_t, std::milli> ms_sync { 50 };

    constexpr hal::pixel_point psz { 32, 32 }; // Piece size

    constexpr hal::pixel_point tsz = psz / piece::szmul(); // Tile size
    constexpr hal::pixel_point bsz = tsz * 8;              // Board size

    constexpr hal::pixel_point wtsz { 80, 80 };
    constexpr hal::pixel_point wbsz = wtsz * 8;

    // Piece draw coord
    constexpr hal::coord_point pdc = hal::anchor::resolve(hal::anchor::center, hal::pixel_point(tsz / 2), psz);

    // Board colors
    constexpr hal::color bc[] { 0x5e1f00, 0xb36656 };
}

namespace hlp
{
    constexpr hal::color cl_from(piece::pos p)
    {
        return cnst::bc[(p.x % 2) xor (p.y % 2)];
    }

    constexpr hal::sdl::coord_rect P2R(piece::pos p)
    {
        using c       = hal::sdl::coord_t;
        constexpr c s = cnst::tsz.x;

        return { c(p.x * s), c(p.y * s), s, s };
    }
}

class chess
{

public:
    chess()
        : m_video { m_engine }
        , m_image { hal::image_loader::png }
        , m_input { m_engine }
        , m_wnd { m_video, "HalChess", { hal::pixel_t(cnst::wbsz.x * 1.2), cnst::wbsz.y }, {} }
        , m_rnd { m_wnd, { hal::renderer::accelerated } }
        , m_pieces { m_rnd, m_image.load("assets/pieces.png") }
        , m_canvas { m_rnd, cnst::bsz }
        , m_nowChosen { piece::invalid_pos(), piece::invalid_pos() }
        , m_whoseTurn { piece::white }
    {
        reset_pieces();
        set_team(piece::white);

        this->draw_canvas();
    }

    bool update()
    {
        const auto tp = std::chrono::steady_clock::now() + cnst::ms_sync;

        this->events();

        std::this_thread::sleep_until(tp);

        return m_input.update();
    }

private:
    template <std::size_t N>
    using path_vec = lyo::static_vector<piece::pos, N>;

    void events()
    {
        const auto set_highlights = [this](piece::pos src, std::span<const piece::pos> i)
        {
            m_highlighted.clear();
            m_highlighted.push_back(src);

            std::copy(i.begin(), i.end(), std::back_inserter(m_highlighted));

            {
                hal::target_lock tl { m_rnd, m_canvas };
                hal::color_lock  cl { m_rnd, { 0x00FF00, 128 } };

                for (auto pos : m_highlighted)
                {
                    using c = hal::sdl::coord_t;

                    constexpr hal::pixel_t tsz = cnst::tsz.x;

                    m_rnd.fill_rect({ c(pos.x * tsz), c(pos.y * tsz), tsz, tsz });
                }
            }

            this->draw_canvas();
        };

        if (m_input.pressed(hal::button::left_mouse))
        {
            const hal::pixel_point pt { m_input.mouse() };

            if (pt >= cnst::wbsz)
                return;

            const piece::pos tile_clicked(pt.x / cnst::wtsz.x, pt.y / cnst::wtsz.y);

            piece& here { this->piece_at(tile_clicked) };

            if (moving())
            {
                // Have we chosen any valid position?
                for (piece::pos p : cnst::moves.get(piece_at(m_nowChosen), m_nowChosen))
                {
                    if (tile_clicked == p) // We have!
                    {
                        HAL_PRINT("Moving from ", m_nowChosen, " to ", tile_clicked);

                        here = std::move(this->piece_at(m_nowChosen));

                        if (here.is(piece::pawn) && tile_clicked.y == (here.tm == piece::white ? 0 : 7))
                            here.tp = piece::queen;

                        this->switch_team();

                        HAL_PRINT("Turn: ", piece::strteam(m_whoseTurn));

                        clear_highlights();
                        m_nowChosen.x = piece::invalid_pos();

                        return;
                    }
                }

                // Are we selecting another piece?
                if (here.friendly(m_whoseTurn) && tile_clicked != m_nowChosen)
                {
                    clear_highlights();
                    set_highlights(tile_clicked, cnst::moves.get(here, tile_clicked));

                    m_nowChosen = tile_clicked;

                    HAL_PRINT("Switching move to ", here.id(), " at ", tile_clicked);
                }

                else
                {
                    clear_highlights();
                    m_nowChosen.x = piece::invalid_pos();
                }
            }

            else // Not moving
            {
                if (here.friendly(m_whoseTurn)) // Can move, set things accordingly
                {
                    HAL_PRINT("Switching move to ", here.id(), " at ", tile_clicked);

                    m_nowChosen = tile_clicked;

                    set_highlights(tile_clicked, cnst::moves.get(here, tile_clicked));
                }
            }
        }
    }

    void set_team(piece::team tm)
    {
        m_whoseTurn = tm;

        m_rnd.set_draw_color(0xFFFFFF * !tm);
        m_rnd.clear();
    }

    void switch_team()
    {
        this->set_team(!m_whoseTurn);
    }

    constexpr bool moving() const
    {
        return m_nowChosen.x != piece::invalid_pos();
    }

    constexpr hal::pixel_point tile_at(piece::pos p) const
    {
        return {
            lyo::cast<hal::pixel_t>(p.x * cnst::tsz.x),
            lyo::cast<hal::pixel_t>(p.y * cnst::tsz.y)
        };
    }

    // Texture alignment
    constexpr static hal::pixel_rect get_src(piece::team tm, piece::type tp)
    {
        using namespace hal::literals;
        return { hal::pixel_t(tp * 32), hal::pixel_t(tm * 32), 32_px, 32_px };
    }

    constexpr hal::pixel_point get_dst(piece::pos p) const
    {
        return { hal::pixel_t(cnst::tsz.x * p.x), hal::pixel_t(cnst::tsz.y * p.y) };
    }

    // Piece indexing
    constexpr piece& piece_at(piece::pos pos)
    {
        return m_boardState[pos.y][pos.x];
    }

    constexpr const piece& piece_at(piece::pos pos) const
    {
        return m_boardState[pos.y][pos.x];
    }

    // Drawing functions
    void compose_board()
    {
        using c = hal::sdl::coord_t;

        std::array<hal::sdl::coord_rect, 32> m_rects;

        lyo::u8 idx { 0 };

        for (lyo::u8 y { 0 }; y < 8; ++y)
        {
            for (lyo::u8 x = y % 2; x < 8; x += 2, ++idx)
            {
                m_rects[idx] = { c(x * cnst::tsz.x), c(y * cnst::tsz.y), cnst::tsz.x, cnst::tsz.y };
            }
        }

        hal::color_lock cl { m_rnd, cnst::bc[0] };
        m_rnd.fill_rects(m_rects);
        idx = 0;

        for (lyo::u8 y { 0 }; y < 8; ++y)
        {
            for (lyo::u8 x { !(y % 2) }; x < 8; x += 2, ++idx)
            {
                m_rects[idx] = { c(x * cnst::tsz.x), c(y * cnst::tsz.y), cnst::tsz.x, cnst::tsz.y };
            }
        }

        cl.set(cnst::bc[1]);
        m_rnd.fill_rects(m_rects);
    }

    void draw_canvas()
    {
        constexpr hal::coord_rect dst { cnst::wbsz };

        m_rnd.draw(m_canvas).to(dst)();

        m_rnd.present();
    }

    // Initial piece drawing routine
    void draw_pieces()
    {
        hal::target_lock tl { m_rnd, m_canvas };

        for (piece::pos p; p.y < 8; ++p.y)
        {
            for (p.x = 0; p.x < 8; ++p.x)
            {
                const piece pc { piece_at(p) };

                if (!pc.valid())
                    continue;

                draw_piece(pc, { hal::sdl::coord_t(p.x * cnst::tsz.x), hal::sdl::coord_t(p.y * cnst::tsz.y) });
            }
        }
    }

    // This function creates it own target lock
    void clear_highlights()
    {
        {
            hal::target_lock tl { m_rnd, m_canvas };
            hal::color_lock  lock { m_rnd };

            // repaint highlighted tiles
            for (auto tile : m_highlighted)
            {
                const hal::sdl::coord_rect dst = hlp::P2R(tile);

                // Draw the background first
                lock.set(hlp::cl_from(tile));
                m_rnd.fill_rect(dst);

                const piece pc { piece_at(tile) };

                if (!pc.valid())
                    continue;

                // Then, the ONE PIECE IS REEEEAAALLLLL
                draw_piece(pc, dst.pos);
            }
        }

        draw_canvas();
    }

    // This function assumes that the target is set
    void draw_piece(piece pc, hal::sdl::coord_point dst)
    {
        m_rnd.draw(m_pieces).from(get_src(pc.tm, pc.tp)).to(dst + cnst::pdc)();
    }

    void reset_pieces()
    {
        using pa   = std::array<piece::pos, 8>;
        using pair = std::pair<lyo::u8, pa>;

        const auto push_list = [this](const std::initializer_list<pair>& pr, piece::team tm)
        {
            for (lyo::u8 t { piece::rook }; t != piece::amount; ++t)
            {
                const pair& curr { *(pr.begin() + t) };

                for (lyo::u8 i { 0 }; i < curr.first; ++i)
                {
                    this->piece_at(curr.second[i]) = { piece::type(t), tm };
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
            { 1, { { { 3, 7 } } } },                                                                      // queen
            { 1, { { { 4, 7 } } } },                                                                      // king
            { 8, { { { 0, 6 }, { 1, 6 }, { 2, 6 }, { 3, 6 }, { 4, 6 }, { 5, 6 }, { 6, 6 }, { 7, 6 } } } } // pawn
        };

        push_list(black, piece::black);
        push_list(white, piece::white);

        hal::target_lock lock { m_rnd, m_canvas };

        this->compose_board();
        this->draw_pieces();
    }

    // All positions and their respective moves
    std::array<std::array<piece, 8>, 8> m_boardState;

    // The current highlighted moves
    lyo::static_vector<piece::pos, 28> m_highlighted;

    LYO_NOSIZE hal::engine m_engine;
    LYO_NOSIZE hal::video m_video;
    LYO_NOSIZE hal::image_loader m_image;

    hal::input_handler m_input;

    hal::window   m_wnd;
    hal::renderer m_rnd;

    const hal::texture m_pieces;

    hal::target_texture m_canvas;

    piece::pos m_nowChosen;

    piece::team m_whoseTurn;
};

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    chess g;

    while (g.update())
        ;

    return EXIT_SUCCESS;
}