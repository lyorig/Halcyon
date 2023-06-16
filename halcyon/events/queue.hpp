#pragma once

#include <halcyon/types.hpp>
#include <lyo/timer.hpp>
#include <queue>
#include <variant>

/* queue.hpp:
   A rather convoluted, but working system for queueing operations
   on an object. By default, only timing is supported, but a second template
   parameter can be specified (taking a const& in the constructor), which can
   be queued in a lambda -> bool, providing an an alternative way to decide whether
   or not the event should be executed and popped. I'm not touching this again, BTW. */

namespace hal
{
    template <typename T>
    struct cond_delayer
    {
        using type = const T&;
    };

    template <>
    struct cond_delayer<void>
    {
        using type = std::monostate;
    };

    template <typename T>
    using cond_object = cond_delayer<T>::type;

    template <typename T>
    using cond_func = lyo::func_ptr<bool, const cond_object<T>&>;

    template <typename T = void>
    using holder = std::conditional_t<std::is_void_v<T>, double, std::variant<double, cond_func<T>>>;

    namespace events
    {
        template <typename T, typename Query = void>
        class queue
        {
          public:

            queue(T& object) noexcept :
                m_object { object }
            {
            }

            queue(T& object, cond_object<Query> cond_object) noexcept :
                m_object { object },
                m_cond { cond_object }
            {
            }

            void update() noexcept
            {
                if (!m_queue.empty())
                {
                    const auto& pair { m_queue.front() };

                    if constexpr (!std::is_void_v<Query>)  // (constexpr) Timer + callback queue.
                    {
                        if (std::holds_alternative<double>(pair.second))
                        {
                            const double time { std::get<double>(pair.second) };

                            if (m_timer() >= time)
                            {
                                pair.first(m_object);
                                m_queue.pop();
                                m_timer -= time;
                            }
                        }

                        else
                        {
                            if (std::get<cond_func<Query>>(pair.second)(m_cond))
                            {
                                pair.first(m_object);
                                m_queue.pop();
                                m_timer.reset();
                            }
                        }
                    }

                    else  // (constexpr) Timer-only queue.
                    {
                        const double time { m_queue.front().second };

                        if (m_timer() >= time)
                        {
                            pair.first(m_object);
                            m_queue.pop();
                            m_timer -= time;
                        }
                    }
                }
            }

            void add(hal::callback<T&> func, holder<Query> conditional) noexcept
            {
                m_queue.emplace(func, conditional);
            }

            bool done() const noexcept
            {
                return m_queue.empty();
            }

          private:

            std::queue<std::pair<hal::callback<T&>, holder<Query>>> m_queue;

            lyo::precise_timer m_timer;

            T& m_object;

            [[no_unique_address]] cond_object<Query> m_cond;
        };
    }  // namespace events
}  // namespace hal