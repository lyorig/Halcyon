#pragma once

#include <AL++/debug.hpp>
#include <AL++/types.hpp>
#include <string_view>

// state.hpp:
// Functions for retreiving OpenAL state.

namespace alpp
{
    class state
    {
    public:
        consteval static auto properties()
        {
            return std::array { doppler_factor, speed_of_sound, distance_model };
        }

        enum query_enum
        {
            vendor     = AL_VENDOR,
            version    = AL_VERSION,
            renderer   = AL_RENDERER,
            extensions = AL_EXTENSIONS
        };

        template <property P>
        static prop_t<P> get()
        {
            using tp = prop_t<P>;

            if constexpr (std::is_same_v<tp, float_t>)
                return ALPP_AL_CALL(::alGetFloat, P);

            else if constexpr (std::is_same_v<tp, int_t>)
                return ALPP_AL_CALL(::alGetInteger, P);
        }

        template <property P>
        static void set(const prop_t<P>& val)
        {
            if constexpr (P == doppler_factor)
                return ALPP_AL_CALL(::alDopplerFactor, val);

            else if constexpr (P == speed_of_sound)
                return ALPP_AL_CALL(::alSpeedOfSound, val);

            else
                return ALPP_AL_CALL(::alDistanceModel, val);
        }

        static std::string_view query(query_enum what);
    };
}