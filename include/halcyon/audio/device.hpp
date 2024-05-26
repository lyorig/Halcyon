#pragma once

#include <halcyon/audio/spec.hpp>

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/internal/subsystem.hpp>

#include <halcyon/utility/concepts.hpp>
#include <halcyon/utility/pass_key.hpp>

namespace hal
{
    namespace audio
    {
        class device;

        namespace builder
        {
            class device
            {
            public:
                device(pass_key<proxy::audio>);

                // Choose a specific audio device.
                device& name(std::string_view name);

                // Have this be a capture device.
                device& capture();

                device& spec(const spec& s);

                device& changes(std::initializer_list<change> vals);

                audio::device operator()();
                audio::device operator()(sdl::spec& obtained);

            private:
                sdl::spec   m_spec;
                const char* m_name;
                int         m_allowedChanges;
                bool        m_capture;
            };
        }

        class device
        {
        public:
            using id_t = SDL_AudioDeviceID;

            // Device querying functions return -1 to signalize an unknown
            // amount of devices etc., but don't intend it to be an error.
            constexpr static id_t invalid_id { static_cast<id_t>(-1) };

            device(const char* name, bool capture, const SDL_AudioSpec* desired, SDL_AudioSpec* obtained, int allowed_changes, pass_key<builder::device>);
            ~device();

            template <meta::buffer T>
            void queue(const T& data)
            {
                HAL_ASSERT_VITAL(::SDL_QueueAudio(m_id, std::data(data), static_cast<int>(std::size(data))) == 0, debug::last_error());
            }

            void pause(bool p);

            void lock();
            void unlock();

            id_t id() const;

        private:
            id_t m_id;
        };
    }
}