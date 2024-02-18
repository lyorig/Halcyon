#pragma once

#include <AL++/get_set.hpp>

namespace alpp
{
    template <typename Derived, auto IGet, auto FGet, auto I3Get, auto F3Get, auto ISet, auto FSet, auto I3Set, auto F3Set>
    class object : get_set<Derived, IGet, FGet, I3Get, F3Get, ISet, FSet, I3Set, F3Set>
    {
        using b = get_set<Derived, IGet, FGet, I3Get, F3Get, ISet, FSet, I3Set, F3Set>;

    public:
        template <property P>
        prop_t<P> get() const
        {
            return b::template get<P>(m_id);
        }

        template <property P>
        void set(const prop_t<P>& value)
        {
            b::template set<P>(value, m_id);
        }

        uint_t id() const { return m_id; }

    protected:
        uint_t m_id;
    };
}