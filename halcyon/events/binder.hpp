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
                if constexpr (!cfg::performance_mode)
                {
                    bool should_trigger;

                    switch (bind.second.first)
                    {
                        case press:
                            should_trigger = m_input.m_pressed(bind.first);
                            break;

                        case hold:
                            should_trigger = m_input.m_held(bind.first);
                            break;

                        case release:
                            should_trigger = m_input.m_released(bind.first);
                            break;

                        default:
                            should_trigger = false;
                            break;
                    }

                    if (should_trigger)
                        bind.second.second(m_hook);
                }

                else  // Not exactly eye candy, but should be better than branching.
                {
                    if ((*((reinterpret_cast<const input_handler::key_storage*>(&m_input)) + bind.second.first))[static_cast<lyo::u64>(bind.first)])
                        bind.second.second(m_hook);
                }
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