#pragma once

#include "piece.hpp"
#include <halcyon/debug.hpp>
#include <lyo/static_vector.hpp>
#include <ranges>
#include <utility>
#include <vector>

// Screw it. Memory is abundant, let's precalculate EVERYTHING
class preprocessed_moves
{
    // Array + count.
    template <std::size_t N>
    using sub = std::pair<std::array<piece::pos, N>, lyo::u8>;

    template <std::size_t N>
    using vec = std::array<std::array<sub<N>, 8>, 8>;

    template <std::size_t N>
    using pawn_vec = std::array<std::array<sub<N>, 6>, 8>;

    // Helpers
    constexpr static bool vloc(piece::pos where)
    {
        return where.x >= 0 && where.x < 8 && where.y >= 0 && where.y < 8;
    }

    // Traverse a direction, with an upper limit of [step] bounds.
    template <typename Iter>
    constexpr static piece::pos_t traverse_n(piece::pos from, piece::dir step, piece::pos_t n, Iter res)
    {
        Iter r = res;

        for (piece::pos curr(from.x + step.x, from.y + step.y);
             n != 0 && vloc(curr);
             --n, ++r, curr.x += step.x, curr.y += step.y)
            *r = curr;

        return std::distance(res, r);
    }

    // Board traversal
    template <typename Iter>
    constexpr static piece::pos_t traverse(piece::pos from, piece::dir step, Iter res)
    {
        return traverse_n(from, step, std::numeric_limits<piece::pos_t>::max(), res);
    }

    template <typename Iter>
    constexpr static piece::pos_t traverse_one(piece::pos from, std::span<const piece::dir> steps, Iter res)
    {
        Iter r = res;

        for (auto s : steps)
            r += traverse_n(from, s, 1, r);

        return std::distance(res, r);
    }

    template <typename Iter>
    constexpr static piece::pos_t traverse_all(piece::pos from, std::span<const piece::dir> directions, Iter res)
    {
        Iter r = res;

        for (auto step : directions)
            r += traverse(from, step, r);

        return std::distance(res, r);
    }

    // Get the tiles to which a piece can move.
    template <typename C>
    constexpr void push_moves(piece::pos src, piece who, C& res)
    {
        switch (who.tp)
        {
            using enum piece::type;

        case rook:
        {
            constexpr piece::dir dir[] { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
            res.second = traverse_all(src, dir, res.first.data());
            break;
        }
        case knight:
        {
            constexpr piece::dir dir[] { { 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 }, { 1, 2 }, { -1, 2 }, { 1, -2 }, { -1, -2 } };
            res.second = traverse_one(src, dir, res.first.data());
            break;
        }
        case bishop: // goes on vacation, never comes back
        {
            constexpr piece::dir dir[] { { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };
            res.second = traverse_all(src, dir, res.first.data());
            break;
        }
        case queen:
        {
            constexpr piece::dir dir[] { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };
            res.second = traverse_all(src, dir, res.first.data());
            break;
        }
        case king:
        {
            constexpr piece::dir dir[] { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 }, { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } };
            res.second = traverse_one(src, dir, res.first.data());
            break;
        }
        case pawn:
        {
            // { white, black }
            constexpr piece::dir_t y_dirs[2] { -1, 1 };
            constexpr piece::pos_t start_pos[2] { 6, 1 };

            const piece::pos_t steps    = 1 + (src.y == start_pos[who.tm]);
            const piece::dir_t vert_dir = y_dirs[who.tm];

            const piece::dir attack_dirs[2] { { -1, vert_dir }, { 1, vert_dir } };

            res.second = traverse_n(src, { 0, vert_dir }, steps, res.first.data());
            res.second += traverse_one(src, attack_dirs, res.first.data() + res.second);

            break;
        }
        default:
            break;
        }
    }

public:
    enum max
    {
        pawn = 4,

        rook   = 14,
        knight = 8,
        bishop = 13,

        queen = 28,
        king  = 8
    };

    constexpr preprocessed_moves()
    {
        piece::pos p;
        piece      pc;

        pc.tp = piece::pawn;

        // pawns
        for (auto team : { piece::white, piece::black })
            for (p.y = 1, pc.tm = team; p.y < 7; ++p.y)
            {
                for (p.x = 0; p.x < 8; ++p.x)
                {
                    push_moves(p, pc, m_pawns[team][p.x][p.y - 1]);
                }
            }

        // rook
        for (p.y = 0, pc.tp = piece::rook; p.y < 8; ++p.y)
        {
            for (p.x = 0; p.x < 8; ++p.x)
            {
                push_moves(p, pc, m_rook[p.x][p.y]);
            }
        }

        // knight
        for (p.y = 0, pc.tp = piece::knight; p.y < 8; ++p.y)
        {
            for (p.x = 0; p.x < 8; ++p.x)
            {
                push_moves(p, pc, m_knight[p.x][p.y]);
            }
        }

        // bishop
        for (p.y = 0, pc.tp = piece::bishop; p.y < 8; ++p.y)
        {
            for (p.x = 0; p.x < 8; ++p.x)
            {
                push_moves(p, pc, m_bishop[p.x][p.y]);
            }
        }

        // queen
        for (p.y = 0, pc.tp = piece::queen; p.y < 8; ++p.y)
        {
            for (p.x = 0; p.x < 8; ++p.x)
            {
                push_moves(p, pc, m_queen[p.x][p.y]);
            }
        }

        // king
        for (p.y = 0, pc.tp = piece::king; p.y < 8; ++p.y)
        {
            for (p.x = 0; p.x < 8; ++p.x)
            {
                push_moves(p, pc, m_king[p.x][p.y]);
            }
        }
    }

    constexpr std::span<const piece::pos> get(piece pc, piece::pos src) const
    {
        switch (pc.tp)
        {
        case piece::rook:
        {
            const auto& ret = m_rook[src.x][src.y];
            return { ret.first.data(), ret.second };
        }
        case piece::knight:
        {
            const auto& ret = m_knight[src.x][src.y];
            return { ret.first.data(), ret.second };
        }
        case piece::bishop:
        {
            const auto& ret = m_bishop[src.x][src.y];
            return { ret.first.data(), ret.second };
        }
        case piece::queen:
        {
            const auto& ret = m_queen[src.x][src.y];
            return { ret.first.data(), ret.second };
        }
        case piece::king:
        {
            const auto& ret = m_king[src.x][src.y];
            return { ret.first.data(), ret.second };
        }
        case piece::pawn:
        {
            const auto& ret = m_pawns[pc.tm][src.x][src.y - 1];
            return { ret.first.data(), ret.second };
        }
        default:
            std::unreachable();
        }
    }

private:
    std::array<vec<pawn>, 2> m_pawns;

    vec<rook>   m_rook;
    vec<knight> m_knight;
    vec<bishop> m_bishop;
    vec<queen>  m_queen;
    vec<king>   m_king;
};