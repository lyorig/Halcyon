#include <quest/camera.hpp>

using namespace quest;

hal::coord_point camera::transform(coord pos, meter_t obj_height, hal::pixel_t renderer_height) const
{
    pos.y = renderer_height - pos.y - obj_height;

    return pos;
}
