#include "api.h"
// #define DEBUG
using namespace pros;

#ifndef _HAWS_CONFIG_H_
#define _HAWS_CONFIG_H_
#define CONSTRAIN(x, lower, upper) ((x)<(lower)?(lower):((x)>(upper)?(upper):(x)))

#define controller Controller(pros::E_CONTROLLER_MASTER)

const pros::MotorGroup motor_group_left({-4,-8,-10});
const pros::MotorGroup motor_group_right({3,6,9});

const pros::MotorGroup motor_group_lift({-13,14});
const pros::MotorGroup motor_group_roller({-17,18});
const pros::Motor motor_clip(11);
const pros::Motor motor_toggle(-19);

const Imu sensor_gyro(15);   // 陀螺仪
const Rotation sensor_lift(-20);  // 旋转 -- 升降

const pros::adi::DigitalOut digit_toggle('B',false);


#endif