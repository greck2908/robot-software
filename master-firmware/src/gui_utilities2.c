#include "base/base_controller.h"
#include "gui_utilities2.h"

robot_position_t get_robot_position(void)
{
    robot_position_t pos;
    pos.x = position_get_x_s16(&robot.pos);
    pos.y = position_get_y_s16(&robot.pos);
    pos.a = position_get_a_deg_s16(&robot.pos);
    return pos;
}
