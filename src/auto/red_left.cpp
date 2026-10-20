#include "haws/auto.h"

using namespace chassis;
using namespace lift;

void redLeft1() {
    sensor_lift.reset_position();
    digit_toggle.set_value(false);
    // 翻滚筒
    motor_toggle.move_relative(2000, 600);//1800，600
    pros::delay(400);
    lift::liftToPosition(200);


    // 放预装
    chassis::moveEnc(730, 3000);
    lift::liftToPosition(1200);
    chassis::turnGyro(82, 3000, 100, E_MOTOR_BRAKE_HOLD);
    chassis::moveEnc(430, 1800);
    // 顶死了退一点
    chassis::move(30, 30);
    pros::delay(50);//200
    chassis::move(-20, -20);
    pros::delay(50);
    chassis::move(0, 0);
    // 放pin
    lift::lift(-55);
    pros::delay(200);
    lift::lift(0);
    motor_clip.move(-127);
    pros::delay(300);

    // 一对 cup+pin
    //对位
    chassis::moveEnc(-410, 3000);//-550
    //motor_clip.move(0);
    chassis::turnGyro(50, 3000);
    motor_clip.move(-127);
    motor_group_roller.move(127);
    chassis::moveEnc(850, 3500);
    chassis::moveEnc(200,1000,70);//40
    // 取
    motor_clip.move(127);
    //motor_group_roller.move(127);
    pros::delay(200);
    lift::liftToPosition(200);
    motor_clip.move(0);
    // 对底座
    chassis::moveEnc(400, 3000);
    lift::liftToPosition(3000);
    chassis::turnGyro(170, 3000);
    chassis::moveEnc(550, 3000);
    chassis::move(20, 20);
    pros::delay(100);//400
    // 放
    lift::liftToPosition(1620, false);//1600
    motor_group_roller.move(0);
    pros::delay(50);
    motor_clip.move(-127);
    pros::delay(100);//300

    // 第二对 cup+pin
    // 对位
    chassis::moveEnc(-550, 3000);
    lift::liftToPosition(0);
    chassis::turnGyro(140.6, 3500);
    lift::lift(-10);
    motor_group_roller.move(50);
    chassis::moveEnc(1000, 3000);
     chassis::moveEnc(200,1000,70);//40
    // 取
    motor_clip.move(127);
    motor_group_roller.move(127);
    pros::delay(200);
    lift::liftToPosition(200);
    motor_clip.move(0);
    // 对底座
    chassis::moveEnc(150, 3000);
    lift::liftToPosition(3750);
    chassis::turnGyro(252, 3000);
    chassis::moveEnc(650, 1500);
    motor_group_roller.move(0);
    // 放
    lift::liftToPosition(3200, false);
    //pros::delay(100);
    motor_clip.move(-127);
    pros::delay(200);
    chassis::move(-70, -70);
    pros::delay(300);
    chassis::move(-40, -40);
    pros::delay(200);
    chassis::move(0, 0);
}

void redLeft2() {
    sensor_lift.reset_position();
    digit_toggle.set_value(false);
    // 翻滚筒
    motor_toggle.move_relative(2000, 600);//1800，600
    pros::delay(400);
    lift::liftToPosition(200);


    // 放预装
    chassis::moveEnc(730, 3000);
    lift::liftToPosition(1200);
    chassis::turnGyro(82, 3000, 100, E_MOTOR_BRAKE_HOLD);
    chassis::moveEnc(430, 1800);
    // 顶死了退一点
    chassis::move(30, 30);
    pros::delay(50);//200
    chassis::move(-20, -20);
    pros::delay(50);
    chassis::move(0, 0);
    // 放pin
    lift::lift(-55);
    pros::delay(200);
    lift::lift(0);
    motor_clip.move(-127);
    pros::delay(300);

    // 一对 cup+pin
    //对位
    chassis::moveEnc(-800, 3000);//-550
    motor_clip.move(0);
}