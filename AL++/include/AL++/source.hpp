#pragma once

#include <AL++/object.hpp>

namespace alpp
{
    class buffer;

    // A source point in 3D space from which the sound eminates.
    class source : public object<source, ::alGetSourcei, ::alGetSourcef, ::alGetSource3i, ::alGetSource3f, ::alSourcei, ::alSourcef, ::alSource3i, ::alSource3f>
    {
    public:
        static consteval auto properties()
        {
            return std::array { pitch, gain, max_distance, rolloff_factor, reference_distance, min_gain, max_gain, cone_outer_gain, cone_inner_angle, cone_outer_angle, position, velocity, direction, relative_source, source_type, looping, buffer_id, source_state, queued_buffers, processed_buffers, second_offset, sample_offset, byte_offset };
        }

        source();
        ~source();

        void attach(const buffer& buf);

        void play();
        void pause();
        void rewind();

        bool playing() const;
        bool valid() const;
    };
}