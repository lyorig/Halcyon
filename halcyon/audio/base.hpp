#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <algorithm>
#include <array>
#include <halcyon/debug.hpp>
#include <lyo/concepts.hpp>
#include <lyo/types.hpp>

#ifdef HAL_DEBUG_ENABLED
    #define HAL_AL_CALL(func, ...)       hal::audio::al::call(__PRETTY_FUNCTION__, #func, func, __VA_ARGS__)
    #define HAL_ALC_CALL(dev, func, ...) hal::audio::alc::call(__PRETTY_FUNCTION__, #func, dev, func, __VA_ARGS__)
#else
    #define HAL_AL_CALL(func, ...)       func(__VA_ARGS__)
    #define HAL_ALC_CALL(func, dev, ...) func(__VA_ARGS__)
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

        max_distance       = AL_MAX_DISTANCE,       // float
        rolloff_factor     = AL_ROLLOFF_FACTOR,     // float
        reference_distance = AL_REFERENCE_DISTANCE, // float
        outer_cone_gain    = AL_CONE_OUTER_GAIN,    // float

        frequency = AL_FREQUENCY, // int
        bit_depth = AL_BITS,      // int
        channels  = AL_CHANNELS,  // int

        second_offset = AL_SEC_OFFSET,  // float
        sample_offset = AL_BYTE_OFFSET, // int
        byte_offset   = AL_BYTE_OFFSET, // int

        inner_cone_angle = AL_CONE_INNER_ANGLE, // float
        outer_cone_angle = AL_CONE_OUTER_ANGLE, // float

        position  = AL_POSITION,  // float[3]
        velocity  = AL_VELOCITY,  // float[3]
        direction = AL_DIRECTION, // float[3]

        looping         = AL_LOOPING,         // boolean (int)
        relative_source = AL_SOURCE_RELATIVE, // boolean (int)

        source_state = AL_SOURCE_STATE, // enum (int)
        source_type  = AL_SOURCE_TYPE,  // enum (int)

        buffer_id = AL_BUFFER, // int
        size      = AL_SIZE,   // int

        processed_buffers = AL_BUFFERS_PROCESSED, // int
        queued_buffers    = AL_BUFFERS_QUEUED     // int
    };

    enum format
    {
        mono8  = AL_FORMAT_MONO8,
        mono16 = AL_FORMAT_MONO16,

        stereo8  = AL_FORMAT_STEREO8,
        stereo16 = AL_FORMAT_STEREO16
    };

    constexpr std::array int_props { size, processed_buffers, queued_buffers, buffer_id, source_state, looping, relative_source, source_type, sample_offset, byte_offset, frequency, bit_depth, channels };
    constexpr std::array float_props { gain, pitch, min_gain, max_gain, inner_cone_angle, outer_cone_angle, max_distance, rolloff_factor, reference_distance, outer_cone_gain, second_offset };

    constexpr std::array float3_props { position, velocity, direction };

    template <std::size_t N>
    constexpr bool contains(const std::array<property, N>& arr, property p)
    {
        return std::find(arr.begin(), arr.end(), p) != arr.end();
    }

    template <property P>
    using prop_t = std::conditional_t<contains(int_props, P),
        int_t, std::conditional_t<contains(float_props, P), float_t, std::conditional_t<contains(float3_props, P), fcoord, void>>>;

    namespace al
    {
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
    }

    namespace alc
    {
        std::string error_string(ALenum err);
        void        check_errors(const char* func, const char* al_func, ALCdevice* dev);

        template <typename Ret, typename... FuncArgs, typename... GivenArgs>
        void call(const char* func_name, const char* al_func_name, ALCdevice* dev, lyo::func_ptr<Ret, FuncArgs...> func, GivenArgs&&... args)
            requires(lyo::is_present_v<Ret, void, bool_t> && sizeof...(FuncArgs) == sizeof...(GivenArgs) && (std::is_convertible_v<GivenArgs, FuncArgs> && ...))
        {
            func(std::forward<GivenArgs>(args)...);
            check_errors(func_name, al_func_name, dev);
        }
    }
}