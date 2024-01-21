#pragma once

#include <halcyon/types/point.hpp>

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
        : tp { invalid }
        , tm {} {

        };

    constexpr piece(const piece&) = default;

    constexpr piece(type t1, team t2)
        : tp { t1 }
        , tm { t2 }
    {
    }

    constexpr piece(piece&& mv) noexcept
        : tp { mv.tp }
        , tm { mv.tm }
    {
        if (this == &mv)
            return;

        mv.tp = invalid;
    }

    constexpr piece& operator=(const piece&) = default;

    constexpr void operator=(piece&& mv) noexcept
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