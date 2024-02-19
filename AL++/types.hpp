#pragma once

#define OPENAL_DEPRECATED // It ain't for me, at least not yet!!!

#include <al.h>
#include <alc.h>
#include <algorithm>
#include <array>
#include <lyo/concepts.hpp>

namespace alpp
{
    template <lyo::arithmetic T>
    struct point3
    {
        T x {}, y {}, z {};
    };

    using int_t  = ALint;
    using uint_t = ALuint;
    using enum_t = ALenum;

    using size_t = ALsizei;

    using bool_t  = ALboolean;
    using byte_t  = ALbyte;
    using ubyte_t = ALubyte;

    using float_t = ALfloat;

    using coordi = point3<int_t>;
    using coordf = point3<float_t>;

    enum property : enum_t
    {
        gain  = AL_GAIN,  // float
        pitch = AL_PITCH, // float

        min_gain = AL_MIN_GAIN, // float
        max_gain = AL_MAX_GAIN, // float

        max_distance       = AL_MAX_DISTANCE,       // float
        rolloff_factor     = AL_ROLLOFF_FACTOR,     // float
        reference_distance = AL_REFERENCE_DISTANCE, // float
        cone_outer_gain    = AL_CONE_OUTER_GAIN,    // float

        frequency = AL_FREQUENCY, // int
        bit_depth = AL_BITS,      // int
        channels  = AL_CHANNELS,  // int

        second_offset = AL_SEC_OFFSET,  // float
        sample_offset = AL_BYTE_OFFSET, // int
        byte_offset   = AL_BYTE_OFFSET, // int

        cone_inner_angle = AL_CONE_INNER_ANGLE, // float
        cone_outer_angle = AL_CONE_OUTER_ANGLE, // float

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
        queued_buffers    = AL_BUFFERS_QUEUED,    // int

        // state variables
        doppler_factor = AL_DOPPLER_FACTOR, // float
        speed_of_sound = AL_SPEED_OF_SOUND, // float
        distance_model = AL_DISTANCE_MODEL  // enum (int)
    };

    enum distance_model : enum_t
    {
        linear         = AL_LINEAR_DISTANCE,
        linear_clamped = AL_LINEAR_DISTANCE_CLAMPED,

        inverse         = AL_INVERSE_DISTANCE,
        inverse_clamped = AL_INVERSE_DISTANCE_CLAMPED,

        exponent         = AL_EXPONENT_DISTANCE,
        exponent_clamped = AL_EXPONENT_DISTANCE_CLAMPED
    };

    enum format : enum_t
    {
        mono8  = AL_FORMAT_MONO8,
        mono16 = AL_FORMAT_MONO16,

        stereo8  = AL_FORMAT_STEREO8,
        stereo16 = AL_FORMAT_STEREO16
    };

    constexpr std::array int_props {
        size,
        processed_buffers,
        queued_buffers,
        buffer_id,
        source_state,
        looping,
        relative_source,
        source_type,
        sample_offset,
        byte_offset,
        frequency,
        bit_depth,
        channels,
        distance_model
    };

    constexpr std::array float_props {
        gain,
        pitch,
        min_gain,
        max_gain,
        cone_inner_angle,
        cone_outer_angle,
        max_distance,
        rolloff_factor,
        reference_distance,
        cone_outer_gain,
        second_offset,
        doppler_factor,
        speed_of_sound
    };

    constexpr std::array float3_props {
        position,
        velocity,
        direction
    };

    template <std::size_t N>
    constexpr bool contains(const std::array<property, N>& arr, property p)
    {
        return std::find(arr.begin(), arr.end(), p) != arr.end();
    }

    template <property P>
    using prop_t = std::conditional_t<contains(int_props, P),
        int_t, std::conditional_t<contains(float_props, P), float_t, std::conditional_t<contains(float3_props, P), coordf, void>>>;
}