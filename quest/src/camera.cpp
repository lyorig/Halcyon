#include <quest/camera.hpp>

using namespace quest;

void camera::update(delta_t elapsed)
{
    pos.update(elapsed);
}

hitbox camera::transform(hitbox hbx, hal::pixel_point rsz) const
{
    const auto here = pos.value();

    hbx.pos.x += rsz.x / 2.0 - here.x;
    hbx.pos.y = rsz.y - hbx.pos.y - hbx.size.y - (rsz.y / 2.0 - here.y);

    return hbx;
}