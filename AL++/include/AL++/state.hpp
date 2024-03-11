#pragma once

#include <AL++/debug.hpp>
#include <AL++/types.hpp>
#include <string>
#include <string_view>

// state.hpp:
// Functions for retreiving OpenAL state.

namespace alpp
{
    namespace state
    {
        consteval auto properties()
        {
            return std::array { doppler_factor, speed_of_sound, distance_model };
        }

        enum query_type
        {
            vendor     = AL_VENDOR,
            version    = AL_VERSION,
            renderer   = AL_RENDERER,
            extensions = AL_EXTENSIONS
        };

        enum null_alc_query_type
        {
            all_outputs = ALC_DEVICE_SPECIFIER,
            all_inputs  = ALC_CAPTURE_DEVICE_SPECIFIER
        };

        template <property P>
        prop_t<P> get()
        {
            using tp = prop_t<P>;

            if constexpr (std::is_same_v<tp, float_t>)
                return ALPP_AL_CALL(::alGetFloat, P);

            else if constexpr (std::is_same_v<tp, int_t>)
                return ALPP_AL_CALL(::alGetInteger, P);
        }

        template <property P>
        void set(const prop_t<P>& val)
        {
            if constexpr (P == doppler_factor)
                return ALPP_AL_CALL(::alDopplerFactor, val);

            else if constexpr (P == speed_of_sound)
                return ALPP_AL_CALL(::alSpeedOfSound, val);

            else
                return ALPP_AL_CALL(::alDistanceModel, val);
        }

        std::string_view query(query_type what);
        std::string      query(null_alc_query_type what);
    };
}