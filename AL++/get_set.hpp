#pragma once

#include <AL++/debug.hpp>
#include <AL++/types.hpp>

namespace alpp
{
    template <typename Derived, auto IGet, auto FGet, auto I3Get, auto F3Get, auto ISet, auto FSet, auto I3Set, auto F3Set>
    struct get_set
    {
        template <property P, typename... Args>
        static prop_t<P> get(Args&&... args)
            requires(contains(Derived::properties(), P))
        {
            using tp = prop_t<P>;

            tp ret {};

            if constexpr (std::is_same_v<tp, int_t>)
            {
                ALPP_AL_CALL(IGet, std::forward<Args>(args)..., static_cast<enum_t>(P), &ret);
            }

            else if constexpr (std::is_same_v<tp, float_t>)
            {
                ALPP_AL_CALL(FGet, std::forward<Args>(args)..., static_cast<enum_t>(P), &ret);
            }

            else if constexpr (std::is_same_v<tp, coordi>)
            {
                ALPP_AL_CALL(I3Get, std::forward<Args>(args)..., static_cast<enum_t>(P), &ret.x, &ret.y, &ret.z);
            }

            else if constexpr (std::is_same_v<tp, coordf>)
            {
                ALPP_AL_CALL(F3Get, std::forward<Args>(args)..., static_cast<enum_t>(P), &ret.x, &ret.y, &ret.z);
            }

            return ret;
        }

        template <property P, typename... Args>
        static void set(const prop_t<P>& value, Args&&... args)
            requires(contains(Derived::properties(), P))
        {
            using tp = prop_t<P>;

            if constexpr (std::is_same_v<tp, int_t>)
            {
                ALPP_AL_CALL(ISet, std::forward<Args>(args)..., static_cast<enum_t>(P), value);
            }

            else if constexpr (std::is_same_v<tp, float_t>)
            {
                ALPP_AL_CALL(FSet, std::forward<Args>(args)..., static_cast<enum_t>(P), value);
            }

            else if constexpr (std::is_same_v<tp, coordi>)
            {
                ALPP_AL_CALL(I3Set, std::forward<Args>(args)..., static_cast<enum_t>(P), value.x, value.y, value.z);
            }

            else if constexpr (std::is_same_v<tp, coordf>)
            {
                ALPP_AL_CALL(F3Set, std::forward<Args>(args)..., static_cast<enum_t>(P), value.x, value.y, value.z);
            }
        }
    };
}