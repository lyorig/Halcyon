#include "level.hpp"

using namespace game;

void level::update(lyo::f64 time) noexcept
{
    for (auto it { m_ents.begin() }; it != m_ents.end(); ++it)
    {
        if ((*it)->can_remove())  // "Swap and pop" the item.
        {
            std::iter_swap(it, m_ents.end() - 1);
            m_ents.pop_back();
            --it;

            continue;
        }

        (*it)->update(time);
    }
}