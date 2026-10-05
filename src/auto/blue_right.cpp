#include "haws/auto.h"

using namespace chassis;
using namespace lift;

void blueRight1() {
    sensor_lift.reset_position();
    digit_toggle.set_value(false);
    // 翻滚筒
    motor_toggle.move_relative(2000, 600);//1800，600
    pros::delay(400);
    lift::liftToPosition(200);


    // 放预装
    chassis::moveEnc(650, 3000);
    lift::liftToPosition(1200);
    chassis::turnGyro(-82, 3000, 100, E_MOTOR_BRAKE_HOLD);
    chassis::moveEnc(430, 1800);
    // 顶死了退一点
    chassis::move(30, 30);
    pros::delay(300);
    chassis::move(-20, -20);
    pros::delay(100);
    chassis::move(0, 0);
    // 放pin
    lift::lift(-30);
    pros::delay(400);
    lift::lift(0);
    motor_clip.move(-127);
    pros::delay(400);


    // 一对 cup+pin
    // 对位
    chassis::moveEnc(-420, 3000);
    motor_clip.move(0);
    chassis::turnGyro(-45, 4000);
    motor_clip.move(-127);
    motor_group_roller.move(50);
    chassis::moveEnc(950, 3500);//950
    // 取
    motor_clip.move(127);
    motor_group_roller.move(127);
    pros::delay(200);
    lift::liftToPosition(200);
    motor_clip.move(0);
    // 对底座
    chassis::moveEnc(450, 3000);
    lift::liftToPosition(2100);
    chassis::turnGyro(-170, 3000);
    chassis::moveEnc(550, 3000);
    chassis::move(20, 20);
    pros::delay(400);
    // 放
    lift::liftToPosition(1100, false);
    motor_group_roller.move(0);
    pros::delay(50);
    motor_clip.move(-127);
    pros::delay(300);

    // 第二对 cup+pin
    // 对位
    chassis::moveEnc(-400, 3000);//500
    lift::liftToPosition(0);
    chassis::turnGyro(-136.6, 3500);
    lift::lift(-10);
    motor_group_roller.move(50);
    chassis::moveEnc(955, 3000);
    // 取
    motor_clip.move(127);
    motor_group_roller.move(127);
    pros::delay(200);
    lift::liftToPosition(200);
    motor_clip.move(0);
    // 对底座
    chassis::moveEnc(400, 3000);
    lift::liftToPosition(3750);
    chassis::turnGyro(-260, 3000);
    chassis::moveEnc(800, 3000);
    // 放
    lift::liftToPosition(3200, false);
    motor_group_roller.move(0);
    pros::delay(50);
    motor_clip.move(-127);
    pros::delay(200);
    chassis::move(-70, -70);
    pros::delay(300);
    chassis::move(-40, -40);
    pros::delay(200);
    chassis::move(0, 0);
}

void blueRight2() {
    sensor_lift.reset_position();
    digit_toggle.set_value(false);
    // 翻滚筒
    motor_toggle.move_relative(2000, 600);//1800，600
    pros::delay(400);
    lift::liftToPosition(200);


    // 放预装
    chassis::moveEnc(650, 3000);
    lift::liftToPosition(1200);
    chassis::turnGyro(-82, 3000, 100, E_MOTOR_BRAKE_HOLD);
    chassis::moveEnc(430, 1800);
    // 顶死了退一点
    chassis::move(30, 30);
    pros::delay(300);
    chassis::move(-20, -20);
    pros::delay(100);
    chassis::move(0, 0);
    // 放pin
    lift::lift(-30);
    pros::delay(400);
    lift::lift(0);
    motor_clip.move(-127);
    pros::delay(400);


    // 一对 cup+pin
    //对位
    chassis::moveEnc(-300, 3000);//-550
}
