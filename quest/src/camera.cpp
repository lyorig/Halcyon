#include <quest/camera.hpp>

using namespace quest;

hitbox camera::transform(hitbox hbx, hal::pixel_point rsz) const
{
    hbx.pos.x += rsz.x / 2.0 - pos.x;
    hbx.pos.y = rsz.y - hbx.pos.y - hbx.size.y - (rsz.y / 2.0 - pos.y);

    return hbx;
}