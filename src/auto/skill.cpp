#include "haws/auto.h"
#include "haws/motorcontrol/lift.h"
#include "pros/motors.h"

void skill() {
    // 满功率走1157距离，超时时间1600ms
    chassis::moveEnc(1157, 1600);
    // 50功率走1157距离，超时时间1600ms
    chassis::moveEnc(1157, 1600, 50);
    // 50功率走1157距离，超时时间1600ms，结束后不刹车
    chassis::moveEnc(1157, 1600, 50, pros::E_MOTOR_BRAKE_INVALID);

    // 升降至高度3000，异步执行，后续步骤不等待
    lift::liftToPosition(3000);
    // 升降至高度5000，同步执行，此步骤执行完成后执行下一步
    lift::liftToPosition(5000, false);

    // 翻滚筒的电机，旋转300度，速度100
    motor_toggle.move_relative(300, 100);
}
