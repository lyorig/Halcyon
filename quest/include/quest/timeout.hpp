#pragma once

#include <future>
#include <vector>

namespace quest
{
    class timeout_manager
    {
    public:
        timeout_manager() = default;

        template <typename Func, typename Rep, typename Period>
            requires std::is_invocable_v<Func>
        void add(const std::chrono::duration<Rep, Period>& timeout, Func&& f)
        {
            m_ignoreMe = std::async(std::launch::async, [&]()
                { std::this_thread::sleep_for(timeout); f(); });
        }

    private:
        std::future<void> m_ignoreMe;
    };
}
