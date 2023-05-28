#pragma once

#include <halcyon/types.hpp>
#include <lyo/timer.hpp>
#include <queue>
#include <variant>

namespace halcyon
{
    template <typename T>
    struct cond_object
    {
        using type = const T&;
    };

    template <>
    struct cond_object<void>
    {
        using type = std::monostate;
    };

    template <typename T>
    using cond_object_t = cond_object<T>::type;

    template <typename T>
    using cond_func_t = lyo::function<bool, const cond_object_t<T>&>;

    template <typename T = void>
    using holder_t = std::conditional_t<std::is_void_v<T>, double, std::variant<double, cond_func_t<T>>>;

    namespace events
    {
        template <typename T, typename Cond = void>
        class queue
        {
          public:

            queue(T& object) noexcept :
                m_object { object }
            {
            }

            queue(T& object, cond_object_t<Cond> cond_object) noexcept :
                m_object { object },
                m_cond { cond_object }
            {
            }

            void update() noexcept
            {
                if (!m_queue.empty())
                {
                    const auto& pair { m_queue.front() };

                    if constexpr (!std::is_void_v<Cond>)  // (constexpr) Timer + callback queue.
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
                            if (std::get<cond_func_t<Cond>>(pair.second)(m_cond))
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

            void add(callback<T&> func, holder_t<Cond> conditional) noexcept
            {
                m_queue.emplace(func, conditional);
            }

            bool done() const noexcept
            {
                return m_queue.empty();
            }

          private:

            std::queue<std::pair<callback<T&>, holder_t<Cond>>> m_queue;

            lyo::precise_timer m_timer;

            T& m_object;

            [[no_unique_address]] cond_object_t<Cond> m_cond;
        };
    }  // namespace events
}  // namespace halcyon