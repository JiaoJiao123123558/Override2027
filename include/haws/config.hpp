#include "api.h"
// #define DEBUG
using namespace pros;

#ifndef _HAWS_CONFIG_H_
#define _HAWS_CONFIG_H_
#define CONSTRAIN(x, lower, upper) ((x)<(lower)?(lower):((x)>(upper)?(upper):(x)))

#define controller Controller(pros::E_CONTROLLER_MASTER)

const pros::MotorGroup motor_group_left({-18,-11, 16});
const pros::MotorGroup motor_group_right({17,14,-15});

const pros::MotorGroup motor_group_lift({-3,4});
const pros::MotorGroup motor_group_roller({-10, 8});
const pros::Motor motor_clip(21);
const pros::Motor motor_toggle(6);

const Imu sensor_gyro(13);   // 陀螺仪
const Rotation sensor_lift(20);  // 旋转 -- 升降

#endif