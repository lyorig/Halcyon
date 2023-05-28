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

            enum event_t
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
                for (const auto& bind : m_binds)
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
            }

            void bind(SDL_Scancode key, event_t type, callback<Hook&> func) noexcept
            {
                m_binds.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(type, func));
            }

            void unbind(SDL_Scancode key) noexcept
            {
                m_binds.erase(key);
            }

          private:

            std::unordered_map<SDL_Scancode, std::pair<event_t, callback<Hook&>>> m_binds;

            const input_handler& m_input;

            Hook& m_hook;
        };
    }  // namespace events
}  // namespace halcyon