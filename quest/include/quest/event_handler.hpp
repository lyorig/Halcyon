#pragma once

#include <lyo/concepts.hpp>

#include <quest/events.hpp>
#include <variant>
#include <vector>

namespace quest
{
    template <typename T>
    concept event_handler_compatible = std::derived_from<T, event> && lyo::non_cv<T>;

    template <event_handler_compatible... Types>
        requires(sizeof...(Types) > 0)
    class event_handler_tmpl
    {
    public:
        using holder = std::variant<Types...>;

        template <lyo::one_of<Types...> Evt>
        void add(const Evt& event)
        {
            m_events.push_back(event);
        }

        template <lyo::one_of<Types...> Evt>
        void add(Evt&& event)
        {
            m_events.push_back(std::move(event));
        }

        const std::vector<holder>& get()
        {
            return m_events;
        }

    private:
        std::vector<holder> m_events;
    };

    using event_handler = event_handler_tmpl<event>;
}