#pragma once

#include <halcyon/types.hpp>
#include <lyo/timer.hpp>
#include <vector>

namespace halcyon
{
    namespace events
    {
        template <typename T>
        class scheduler
        {
            struct callback_info
            {
                callback_info(callback<T&> func, double interval) noexcept :
                    function { func },
                    interval { interval }
                {
                }

                callback<T&>       function;
                lyo::precise_timer timer;

                const double interval;
            };

          public:

            scheduler(T& object) noexcept :
                m_object { object }
            {
            }

            void add(callback<T&> func, double interval) noexcept
            {
                m_callbacks.emplace_back(func, interval);
            }

            void update() noexcept
            {
                for (auto& cbk : m_callbacks)
                {
                    if (cbk.timer() >= cbk.interval)
                    {
                        cbk.function(m_object);
                        cbk.timer -= cbk.interval;
                    }
                }
            }

          private:

            std::vector<callback_info> m_callbacks;

            T& m_object;
        };
    }  // namespace events
}  // namespace halcyon