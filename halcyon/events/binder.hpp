#pragma once

#include <halcyon/input_handler.hpp>
#include <unordered_map>

namespace halcyon
{
    namespace events
    {
        template <typename Hook>
        class binder
        {
          public:

            using key_type = input_handler::button_t;

            enum event_t : lyo::u8
            {
                press,
                hold,
                release
            };

            binder(const input_handler& input, Hook& hook) noexcept :
                m_input { input },
                m_hook { hook }
            {
            }

            void update() const noexcept
            {
                /* Set to true only in case of an emergency, whatever that might be. */
                constexpr bool performance_mode { false };

                for (const auto& bind : m_binds)
                {
                    if constexpr (!performance_mode)
                    {
                        bool should_trigger;

                        switch (bind.second.first)
                        {
                            case press:
                                should_trigger = m_input.pressed(bind.first);
                                break;

                            case hold:
                                should_trigger = m_input.held(bind.first);
                                break;

                            case release:
                                should_trigger = m_input.released(bind.first);
                                break;

                            default:
                                should_trigger = false;
                                break;
                        }

                        if (should_trigger)
                            bind.second.second(m_hook);
                    }

                    else  // Probably UB. But it's cool, right?
                    {
                        if ((*(((input_handler::key_storage*)&m_input) + bind.second.first))[bind.first])
                            bind.second.second(m_hook);
                    }
                }
            }

            void bind(key_type key, event_t type, callback<Hook&> func) noexcept
            {
                m_binds.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(type, func));
            }

            void unbind(key_type key) noexcept
            {
                m_binds.erase(key);
            }

          private:

            // TODO: Add support for multi-key binds.
            std::unordered_map<key_type, std::pair<event_t, callback<Hook&>>> m_binds;

            const input_handler& m_input;

            Hook& m_hook;
        };
    }  // namespace events
}  // namespace halcyon