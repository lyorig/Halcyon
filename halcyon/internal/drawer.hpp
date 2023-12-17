#pragma once
#include <halcyon/enums/anchor.hpp>
#include <halcyon/types/render.hpp>

namespace hal
{
    template <lyo::arithmetic T>
    constexpr T unset { std::numeric_limits<T>::max() };

    template <typename T, typename Dest_type, typename This = void>
        requires(lyo::is_any_of<Dest_type, SDL::pixel_type, SDL::position_type>())
    class drawer
    {
    protected:
        using st = SDL::pixel_type;
        using dt = Dest_type;

        using spoint = point<st>;
        using srect = rectangle<st>;

        using dpoint = point<dt>;
        using drect = rectangle<dt>;

        using this_ref = std::conditional_t<std::is_void_v<This>, drawer, This>&;

    public:
        [[nodiscard]] drawer(const T& src)
            : m_this { src }
            , m_dst { as_size, src.size() }

        {
            m_src.pos.x = unset<st>;
        }

        // Set where to draw.
        // Discards any previous scaling and anchoring.
        [[nodiscard]] this_ref to(const dpoint& pos)
        {
            m_dst.pos = pos;
            return get_this();
        }

        // Set the destination rectangle.
        // Discards any previous scaling and anchoring.
        [[nodiscard]] this_ref to(const drect& area)
        {
            m_dst = area;
            return get_this();
        }

        // Stretch across the board.
        // Do not use with scaling and anchoring.
        [[nodiscard]] this_ref to(fill_tag)
        {
            m_dst.pos.x = unset<dt>;
            return get_this();
        }

        // Set the source rectangle.
        // Can be called at any time.
        [[nodiscard]] this_ref from(const pixel_area& src)
        {
            m_src = src;
            return get_this();
        }

        // Set the destination's scale.
        // Call after setting the destination and before anchoring.
        [[nodiscard]] this_ref scale(lyo::f64 mul)
        {
            if (m_dst.pos.x != unset<dt>)
                m_dst.size *= mul;
            return get_this();
        }

        // Anchor from the destination position.
        // Call after setting the destination and scaling.
        [[nodiscard]] this_ref anchor(anchor::pos anch)
        {
            if (m_dst.pos.x != unset<dt>)
                m_dst.pos = anchor::resolve(anch, m_dst.pos, m_dst.size);
            return get_this();
        }

    protected:
        this_ref get_this()
        {
            return static_cast<this_ref>(*this);
        }

        const T& m_this;

        drect m_dst;
        srect m_src;
    };
}