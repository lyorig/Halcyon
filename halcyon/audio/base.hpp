#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <halcyon/debug.hpp>
#include <lyo/types.hpp>
#include <string>

#ifdef HAL_DEBUG_ENABLED
    #define HAL_AL_CALL(func, ...) hal::audio::call(__PRETTY_FUNCTION__, #func, func, __VA_ARGS__)
#else
    #define HAL_AL_CALL(func, ...) func##(__VA_ARGS__)
#endif

namespace hal::audio
{
    template <lyo::arithmetic T>
    struct point3
    {
        T x {}, y {}, z {};
    };

    using int_t  = ALint;
    using uint_t = ALuint;

    using ssize_t = ALsizei;

    using bool_t  = ALboolean;
    using enum_t  = ALenum;
    using byte_t  = ALbyte;
    using ubyte_t = ALubyte;

    using float_t = ALfloat;

    using icoord = point3<int_t>;
    using fcoord = point3<float_t>;

    enum property
    {
        gain  = AL_GAIN,  // float
        pitch = AL_PITCH, // float

        min_gain = AL_MIN_GAIN, // float
        max_gain = AL_MAX_GAIN, // float

        inner_cone_angle = AL_CONE_INNER_ANGLE, // float
        outer_cone_angle = AL_CONE_OUTER_ANGLE, // float

        position  = AL_POSITION,  // float[3]
        velocity  = AL_VELOCITY,  // float[3]
        direction = AL_DIRECTION, // float[3]

        looping         = AL_LOOPING,         // boolean
        relative_source = AL_SOURCE_RELATIVE, // boolean

        source_state = AL_SOURCE_STATE, // enum

        buffer_id = AL_BUFFER // uint
    };

    enum format
    {
        mono8  = AL_FORMAT_MONO8,
        mono16 = AL_FORMAT_MONO16,

        stereo8  = AL_FORMAT_STEREO8,
        stereo16 = AL_FORMAT_STEREO16
    };

    constexpr std::array uint_props { buffer_id };
    constexpr std::array float_props { gain, pitch, min_gain, max_gain, inner_cone_angle, outer_cone_angle };
    constexpr std::array enum_props { source_state };

    constexpr std::array float3_props { position, velocity, direction };
    constexpr std::array bool_props { looping, relative_source };

    template <std::size_t N>
    constexpr bool contains(const std::array<property, N>& arr, property p)
    {
        return std::find(arr.begin(), arr.end(), p) != arr.end();
    }

    template <property P>
    using prop_t = std::conditional_t<contains(uint_props, P),
        uint_t,
        std::conditional_t<contains(float_props, P), float_t,
            std::conditional_t<contains(enum_props, P), enum_t,
                std::conditional_t<contains(float3_props, P), fcoord,
                    std::conditional_t<contains(bool_props, P), bool_t, void>>>>>;

    std::string error_string(ALenum err);
    void        check_errors(const char* func, const char* al_func);

    template <typename Ret, typename... FuncArgs, typename... GivenArgs>
    Ret call(const char* func_name, const char* al_func_name, lyo::func_ptr<Ret, FuncArgs...> func, GivenArgs&&... args)
        requires(sizeof...(FuncArgs) == sizeof...(GivenArgs) && (std::is_convertible_v<GivenArgs, FuncArgs> && ...))
    {
        if constexpr (std::is_void_v<Ret>)
        {
            func(std::forward<GivenArgs>(args)...);
            check_errors(func_name, al_func_name);
        }

        else
        {
            const Ret rv { func(std::forward<GivenArgs>(args)...) };
            check_errors(func_name, al_func_name);
            return rv;
        }
    }

    template <auto IGet, auto FGet, auto I3Get, auto F3Get, auto ISet, auto FSet, auto I3Set, auto F3Set>
    struct object
    {
        template <property P, typename... Args>
        prop_t<P> get(Args&&... args)
        {
            using tp = prop_t<P>;

            tp ret;

            if constexpr (std::is_same_v<tp, int_t>)
            {
                HAL_AL_CALL(IGet, std::forward<Args>(args)..., P, &ret);
            }

            else if constexpr (std::is_same_v<tp, float_t>)
            {
                HAL_AL_CALL(FGet, std::forward<Args>(args)..., P, &ret);
            }

            else if constexpr (std::is_same_v<tp, icoord>)
            {
                HAL_AL_CALL(I3Get, std::forward<Args>(args)..., P, &ret.x, &ret.y, &ret.z);
            }

            else if constexpr (std::is_same_v<tp, fcoord>)
            {
                HAL_AL_CALL(F3Get, std::forward<Args>(args)..., P, &ret.x, &ret.y, &ret.z);
            }

            return ret;
        }

        template <property P, typename... Args>
        void set(const prop_t<P>& value, Args&&... args)
        {
            using tp = prop_t<P>;

            if constexpr (std::is_same_v<tp, int_t>)
            {
                HAL_AL_CALL(ISet, std::forward<Args>(args)..., P, value);
            }

            else if constexpr (std::is_same_v<tp, float_t>)
            {
                HAL_AL_CALL(FSet, std::forward<Args>(args)..., P, value);
            }

            else if constexpr (std::is_same_v<tp, icoord>)
            {
                HAL_AL_CALL(I3Set, std::forward<Args>(args)..., P, value.x, value.y, value.z);
            }

            else if constexpr (std::is_same_v<tp, fcoord>)
            {
                HAL_AL_CALL(F3Set, std::forward<Args>(args)..., P, value.x, value.y, value.z);
            }
        }
    };
}