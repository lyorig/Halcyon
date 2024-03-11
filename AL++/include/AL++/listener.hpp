#pragma once

#include <AL++/get_set.hpp>
#include <string_view>

namespace alpp
{

    // This represents a listener. Only one can exist, so do not
    // create this class multiple times.
    class listener : get_set<listener, ::alGetListeneri, ::alGetListenerf, ::alGetListener3i, ::alGetListener3f, ::alListeneri, ::alListenerf, ::alListener3i, ::alListener3f>
    {
    public:
        static consteval auto properties()
        {
            return std::array { gain, position, velocity };
        }

        enum query_type : alc::enum_t
        {
            default_device_name = ALC_DEFAULT_DEVICE_SPECIFIER,
            device_name         = ALC_DEVICE_SPECIFIER,

            openal_extensions = ALC_EXTENSIONS
        };

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

        std::string_view query(query_type what) const;

    private:
        ALCdevice*  m_dev;
        ALCcontext* m_ctx;
    };
}