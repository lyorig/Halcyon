#pragma once

#include <halcyon/audio/object.hpp>

namespace hal::audio
{
    class buffer;

    // A source point in 3D space from which the sound eminates.
    class source : public object<source, ::alGetSourcei, ::alGetSourcef, ::alGetSource3i, ::alGetSource3f, ::alSourcei, ::alSourcef, ::alSource3i, ::alSource3f>
    {
    public:
        static consteval auto properties()
        {
            return std::array { pitch, gain, max_distance, rolloff_factor, reference_distance, min_gain, max_gain, outer_cone_gain, inner_cone_angle, outer_cone_angle, position, velocity, direction, relative_source, source_type, looping, buffer_id, source_state, queued_buffers, processed_buffers, second_offset, sample_offset, byte_offset };
        }

        source();
        ~source();

        void attach(const buffer& buf);

        void play();
        void pause();
        void rewind();

        bool playing();
        bool valid();
    };
}