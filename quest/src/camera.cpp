#include <quest/camera.hpp>

using namespace quest;

hal::coord_point camera::transform(hitbox hbx, hal::pixel_t renderer_height) const
{
    (void)hbx;
    // TODO: This calculation
    // hbx.pos.y = renderer_height - 1;

    return pos;
}
