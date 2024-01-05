#pragma once

#include <ecs/base/atp.hpp>
#include <ecs/base/scene.hpp>

namespace ECS
{
    template <typename Comp_Mgr, template <typename> typename Entity, std::size_t Max_Ents>
    using static_scene = scene<array_template_proxy<Max_Ents>::template type, Comp_Mgr, Entity>;

    template <typename Comp_Mgr, template <typename> typename Entity>
    using dynamic_scene = scene<std::vector, Comp_Mgr, Entity>;
}