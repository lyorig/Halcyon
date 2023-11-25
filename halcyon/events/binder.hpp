#pragma once

#include <halcyon/input_handler.hpp>
#include <halcyon/internal/config.hpp>
#include <halcyon/types/other.hpp>
#include <unordered_map>

namespace hal
{
    template <typename Hook>
    class binder
    {
      public:

        enum event : lyo::u8
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

        // Check all binds. This doesn't call update() on the underlying
        // input handler, you need to do that yourself!
        void update() const noexcept
        {
            for (const auto& bind : m_binds)
            {
                bool should_trigger;

                switch (bind.second.first) // Why is the formatting like this?!?!
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
                                    return;
                    }

                    if (should_trigger)
                        bind.second.second(m_hook);
            }
        }

        void bind(button key, event type, callback<Hook&> func) noexcept
        {
            m_binds.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(type, func));
        }

        void unbind(button key) noexcept
        {
            m_binds.erase(key);
        }

      private:

        // TODO: Add support for multi-key binds.
        std::unordered_map<button, std::pair<event, callback<Hook&>>> m_binds;

        const input_handler& m_input;

        Hook& m_hook;
    };
}  // namespace hal