#include "haws/auto.h"
#include "haws/motorcontrol/chassis.h"
#include "haws/motorcontrol/lift.h"
#include "main.h"
#include "pros/motors.h"

void skill() {
    sensor_lift.reset_position();
    digit_toggle.set_value(false);
    // 翻滚筒
    motor_toggle.move_relative(2000, 600);//1800，600
    pros::delay(400);
    lift::liftToPosition(200);


    // 放预装
    chassis::moveEnc(700, 3000);
    lift::liftToPosition(1100);
    chassis::turnGyro(83, 3000, 100, E_MOTOR_BRAKE_HOLD);
    chassis::moveEnc(430, 1800);
    // 顶死了退一点
    chassis::move(40, 40);
    pros::delay(100);
    chassis::move(0, 0);
    // 放pin
    lift::lift(-60);
    pros::delay(300);
    lift::lift(0);
    motor_clip.move(-130);
    pros::delay(200);


    // 一对 cup+pin
    //对位
    chassis::moveEnc(-420, 3000);//-550
    motor_clip.move(0);
    chassis::turnGyro(48, 3000);
    motor_clip.move(-127);
    motor_group_roller.move(50);
    chassis::moveEnc(850, 3500, 100, pros::E_MOTOR_BRAKE_COAST);
    chassis::moveEnc(200, 1000, 80);
    // 取
    motor_clip.move(127);
    motor_group_roller.move(127);
    pros::delay(200);
    lift::liftToPosition(200);
    motor_clip.move(0);
    // 对底座
    chassis::moveEnc(350, 3000);
    lift::liftToPosition(3000);
    chassis::turnGyro(172, 3000);
    chassis::moveEnc(560, 3000);
    chassis::move(20, 20);
    pros::delay(300);
    // 放
    lift::liftToPosition(1600, false);
    motor_group_roller.move(0);
    pros::delay(50);
    motor_clip.move(-127);
    pros::delay(100);

    // // 第二对 cup+pin
    // // 对位
    chassis::moveEnc(-650, 3000);
    lift::liftToPosition(0);
    chassis::turnGyro(140.6, 3500);
    lift::lift(-10);
    motor_group_roller.move(50);
    chassis::moveEnc(1100, 3000);
    chassis::moveEnc(200,1000,100);//40
    // 取
    motor_clip.move(127);
    motor_group_roller.move(127);
    pros::delay(200);
    lift::liftToPosition(200);
    motor_clip.move(0);
    // 对底座
    chassis::moveEnc(430, 3000);
    lift::liftToPosition(3750);
    chassis::turnGyro(265, 3000);
    chassis::moveEnc(780, 1500);
    // 放
    lift::liftToPosition(3200, false);
    motor_group_roller.move(0);
    pros::delay(100);
    motor_clip.move(-127);
    pros::delay(200);
    

    // 顶墙
    chassis::move(-60, -60);
    pros::delay(400);
    lift::liftToPosition(0);
    chassis::moveDis(chassis::back, 10, 2000, 80);
    chassis::move(-35, -35);
    pros::delay(500);
    chassis::move(0, 0);
    sensor_gyro.set_rotation(270);
    pros::delay(100);

    int posHigh[3] = {4700, 5600, 6500};
    int posLow[3] = {4122, 5200, 6300};
    int delayUp[3] = {200, 500, 800};
    int delayDown[3] = {0, 200, 500};
    for (int i = 0; i < 3; i++) {
        // 取手填 - 1.1
        chassis::moveDis(chassis::back, 55, 1000);
        chassis::turnGyro(186, 2000);
        motor_clip.move(-127);
        motor_group_roller.move(127);
        chassis::moveDis(chassis::front, 195, 2000);
        motor_clip.move(0);
        pros::delay(400);
        lift::liftToPosition(650, false);
        chassis::moveDis(chassis::front, 310, 2500);

        // 放一对 - 1.1
        chassis::turnGyro(265, 2000);
        lift::liftToPosition(posHigh[i]);
        pros::delay(delayUp[i]);
        chassis::moveDis(chassis::back, 800, 1700, 70);
        lift::liftToPosition(posLow[i], false);
        motor_group_roller.move(0);
        pros::delay(100);
        motor_clip.move(-127);
        pros::delay(300);
        chassis::move(-40, -40);
        pros::delay(500);
        lift::liftToPosition(0);
        pros::delay(delayDown[i]);
        chassis::moveDis(chassis::back, 100, 1000);
    }
    
    // 取手填 - 2.1
    chassis::moveDis(chassis::back, 55, 1000);
    chassis::turnGyro(186, 2000);
    motor_clip.move(-127);
    motor_group_roller.move(127);
    chassis::moveDis(chassis::front, 195, 2000);
    motor_clip.move(0);
    pros::delay(400);
    lift::liftToPosition(650, false);
    chassis::moveDis(chassis::front, 310, 2500);

    // 放一对 - 2.1
    chassis::turnGyro(340, 3000);
    lift::liftToPosition(1200, false);
    chassis::moveEnc(800, 2000);
    lift::lift(-60);
    pros::delay(300);
    lift::lift(0);
    motor_clip.move(-127);
    pros::delay(200);
    chassis::moveEnc(-800, 2000);

    // 取手填 - 2.2
    chassis::turnGyro(186, 2000);
    motor_clip.move(-127);
    motor_group_roller.move(127);
    chassis::moveDis(chassis::front, 195, 2000);
    motor_clip.move(0);
    pros::delay(400);
    lift::liftToPosition(650, false);
    chassis::moveDis(chassis::front, 310, 2500);

    // 放一对 - 2.2
    chassis::turnGyro(340, 3000);
    lift::liftToPosition(2100, false);
    chassis::moveEnc(800, 2000);
    lift::liftToPosition(1750, false);
    motor_group_roller.move(0);
    pros::delay(100);
    motor_clip.move(-127);
    pros::delay(300);
    chassis::moveEnc(-400, 2000);

    // 翻toggle
    chassis::turnGyro(320, 2000);
    chassis::moveEnc(1800, 3500);
    chassis::turnGyro(275, 2000);
    chassis::moveDis(chassis::back, 120, 2500);
}
