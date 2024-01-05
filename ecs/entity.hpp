#pragma once

#include <ecs/base/atp.hpp>
#include <ecs/base/entity.hpp>

namespace ECS
{
    template <typename Comp_Mgr>
    using static_entity = entity_base<array_template_proxy<Comp_Mgr::components()>::template type>;
}