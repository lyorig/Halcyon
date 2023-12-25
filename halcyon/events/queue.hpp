#pragma once

#include <halcyon/types/other.hpp>
#include <lyo/timer.hpp>
#include <queue>
#include <variant>

// queue.hpp:
// A rather convoluted, but working system for queueing operations
// on an object. By default, only timing is supported, but a second template
// parameter can be specified (taking a const& in the constructor), which can
// be queued in a lambda -> bool, providing an an alternative way to decide whether
// or not the event should be executed and popped. I'm not touching this again, BTW. <- lie!

namespace hal
{
    template <typename T, typename Query = void>
    class queue;

    template <typename T> // Specialization for the default timer-only queue.
    class queue<T, void>
    {
    public:
        queue(T& object)
            : m_object { object }
        {
        }

        void update()
        {
            if (!m_queue.empty())
            {
                const double time { m_queue.front().second };

                while (m_timer() >= time)
                {
                    m_queue.front().first(m_object);
                    m_queue.pop();
                    m_timer -= time;
                }
            }
        }

        void add(hal::callback<T&> func, double time)
        {
            m_queue.emplace(func, time);
        }

        bool done() const
        {
            return m_queue.empty();
        }

    private:
        std::queue<std::pair<hal::callback<T&>, const double>> m_queue;

        lyo::precise_timer m_timer;

        T& m_object;
    };

    template <typename T, typename Query> // Specialization for a conditional queue.
    class queue
    {
        using cond_func = lyo::func_ptr<bool, const Query&>;
        using holder = std::variant<double, cond_func>;

    public:
        queue(T& object, const Query& query_obj)
            : m_object { object }
            , m_cond { query_obj }
        {
        }

        void update()
        {
            if (!m_queue.empty())
            {
                const auto& pair { m_queue.front() };

                if (std::holds_alternative<double>(pair.second)) // Timer conditional.
                {
                    const double time { std::get<double>(pair.second) };

                    if (m_timer() >= time)
                    {
                        pair.first(m_object);
                        m_queue.pop();
                        m_timer -= time;
                    }
                }

                else // Function conditional.
                {
                    if (std::get<cond_func>(pair.second)(m_cond))
                    {
                        pair.first(m_object);
                        m_queue.pop();
                        m_timer.reset();
                    }
                }
            }
        }

        void add(hal::callback<T&> func, double time)
        {
            m_queue.emplace(func, time);
        }

        void add(hal::callback<T&> func, cond_func conditional)
        {
            m_queue.emplace(func, conditional);
        }

        bool done() const
        {
            return m_queue.empty();
        }

    private:
        std::queue<std::pair<hal::callback<T&>, holder>> m_queue;

        lyo::precise_timer m_timer;

        T& m_object;

        const Query& m_cond;
    };
} // namespace hal