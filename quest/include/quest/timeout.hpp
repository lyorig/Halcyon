#pragma once

#include <future>

namespace quest
{
    template <typename F, typename Rep, typename Period>
    void set_timeout(F&& func, std::chrono::duration<Rep, Period> timeout)
    {
        std::thread t {
            [&]()
            {
                std::this_thread::sleep_for(timeout);
                std::forward<F>(func)();
            }
        };

        t.detach();
    }
}