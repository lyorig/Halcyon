#pragma once

#include <halcyon/types/point.hpp>
#include <string>

struct piece
{
    using pos_t = lyo::u8;
    using dir_t = lyo::i8;

    struct pos
    {
        constexpr pos() = default;

        constexpr pos(pos_t x, pos_t y)
            : x { x }
            , y { y }
        {
        }

        constexpr auto operator<=>(const pos&) const = default;

        pos_t x : 4 {}, y : 4 {};
    };

    using dir = hal::point<dir_t>;

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

    static constexpr std::string_view strteam(team tm)
    {
        return tm == white ? "White" : "Black";
    }

    static constexpr std::string_view strtype(type tp)
    {
        switch (tp)
        {
        case rook:
            return " rook";
        case knight:
            return " knight";
        case bishop:
            return " bishop";
        case queen:
            return " queen";
        case king:
            return " king";
        case pawn:
            return " pawn";
        case amount:
            return " [amount]";
        case invalid:
            return " [invalid]";
        }
    }

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
        return 8;
    }

    constexpr std::string id() const
    {
        return std::string { strteam(tm) } + strtype(tp).data();
    }

    constexpr bool valid() const
    {
        return tp != invalid;
    }

    constexpr bool is(type t) const
    {
        return tp == t;
    }

    constexpr bool friendly(team your_team) const
    {
        return valid() && tm == your_team;
    }

    constexpr bool takeable(team your_team) const
    {
        return valid() && tm != your_team && tp != king;
    }

    constexpr bool operator==(const piece& pc) const = default;

    type tp : 7;
    team tm : 1;
};

constexpr piece::team operator!(piece::team tm)
{
    return piece::team(!bool(tm));
}