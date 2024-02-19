#pragma once

#include <AL++/get_set.hpp>
#include <string_view>

namespace alpp
{
    enum context_query
    {
        default_output_device_name = ALC_DEFAULT_DEVICE_SPECIFIER,
        default_input_device_name  = ALC_CAPTURE_DEVICE_SPECIFIER,

        output_device_name = ALC_DEVICE_SPECIFIER,
        input_device_name  = ALC_CAPTURE_DEVICE_SPECIFIER,

        extensions = ALC_EXTENSIONS
    };

    // This represents a listener. Only one can exist, so do not
    // create this class multiple times.
    class listener : get_set<listener, ::alGetListeneri, ::alGetListenerf, ::alGetListener3i, ::alGetListener3f, ::alListeneri, ::alListenerf, ::alListener3i, ::alListener3f>
    {
    public:
        static consteval auto properties()
        {
            return std::array { gain, position, velocity };
        }

        listener();
        ~listener();

        template <property P>
        prop_t<P> get()
        {
            return get_set::get<P>();
        }

        template <property P>
        void set(const prop_t<P>& value)
        {
            get_set::set<P>(value);
        }

        std::string_view query(context_query what) const;

    private:
        ALCdevice*  m_dev;
        ALCcontext* m_ctx;
    };
}