#include "main.h"
#include "haws/motorcontrol/lift.h"
#include "haws/motorcontrol/chassis.h"
#include "haws/auto.h"
#include "pros/motors.h"
#include "haws/display/logger.h"
#include <cerrno>
#include <cstdio>

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	
    motor_group_left.tare_position_all();
    motor_group_right.tare_position_all();
    sensor_lift.reset_position();
    sensor_gyro.tare_rotation();

    motor_group_left.set_brake_mode_all(E_MOTOR_BRAKE_COAST);
    motor_group_right.set_brake_mode_all(E_MOTOR_BRAKE_COAST);
    motor_group_lift.set_brake_mode_all(E_MOTOR_BRAKE_HOLD);

    Logger::getInstance();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
    chassis::move(0, 0);
    lift::setTaskEnable(false);
    pros::delay(10);
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
int autoSelection = 6;
// bool isRunAuto = false;
const char *autoTitles[] = {
    "技能",
    "红左1",
    "红左2",
    "红右1",
    "红右2",
    "蓝左1",
    "蓝左2",
    "蓝右1",
    "蓝右2"};
void autonomous() {
    // isRunAuto = true;
    switch (autoSelection) {
    case 0:
        skill();
        break;
    case 1:
        redLeft1();
        break;
    case 2:
        redLeft2();
        break;
    case 3:
        redRight1();
        break;
    case 4:
        redRight2();
        break;
    case 5:
        blueLeft1();
        break;
    case 6:
        blueLeft2();
        break;
    case 7:
        blueRight1();
        break;
    case 8:
        blueRight2();
        break;
    }
}

// 是否进入选自动程序
bool isSelectAuto = false;
// 选自动
void autoSelector() {
    controller.print(2, 0, "[ ]%s", autoTitles[autoSelection]);
    while (isSelectAuto) {
    //     // TODO: 显示陀螺仪传感器的值
    //     // controller.print(2, 20, "gyro:%.3f", gyro.get_rotation());
        if (controller.get_digital_new_press(DIGITAL_LEFT)) {
            autoSelection = CONSTRAIN(autoSelection - 1, 0, 8);
            controller.clear_line(2);
            pros::delay(50);
            controller.print(2, 0, "[ ]%s", autoTitles[autoSelection]);
        }
        if (controller.get_digital_new_press(DIGITAL_RIGHT)) {
            autoSelection = CONSTRAIN(autoSelection + 1, 0, 8);
            controller.clear_line(2);
            pros::delay(50);
            controller.print(2, 0, "[ ]%s", autoTitles[autoSelection]);
        }
        if (controller.get_digital_new_press(DIGITAL_B)) {
            break;
        }
        pros::delay(50);
    }
    pros::delay(50);
    controller.print(2, 1, "x");
    isSelectAuto = false;
}

/*
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
bool overThreshold() {
    if (abs(controller.get_analog(ANALOG_LEFT_Y)) > 10) {
        return true;
    }
    if (abs(controller.get_analog(ANALOG_LEFT_X)) > 10) {
        return true;
    }
    if (abs(controller.get_analog(ANALOG_RIGHT_X)) > 10) {
        return true;
    }
    if (abs(controller.get_analog(ANALOG_RIGHT_Y)) > 10) {
        return true;
    }
    return false;
}

void opcontrol() {
    bool clipState = true;
    bool gunState = true;
    int curLiftGear = 0;
    bool chassisLock = false;
    int controllerPrintCount = 0;

    autoSelector();

    pros::lcd::register_btn0_cb(Logger::getInstance().pageUpCallback);
    pros::lcd::register_btn1_cb(chassis::reset);
    pros::lcd::register_btn2_cb(Logger::getInstance().pageDownCallback);

	while(true) {
        // controllerPrintCount++;
        // if (controllerPrintCount > 10) {
        //     controllerPrintCount = 0;
        //     if (!isRunAuto) {
        //         controller.print(2, 0, "%.2f                ", sensor_gyro.get_rotation());
        //     } else {
        //         controller.print(2, 0, chassisLock ? "锁底盘" : "               ");
        //     }
        // }
        pros::lcd::print(6, "enc: %d, gyro: %d, temp: %.1f", chassis::getPosition(), sensor_gyro.get_rotation(), motor_group_left.get_temperature());
        pros::lcd::print(7, "lift rotate: %d, temp: %.1f", sensor_lift.get_position(), motor_group_lift.get_temperature());
		int ch3 = controller.get_analog(ANALOG_LEFT_Y);
		int ch1 = controller.get_analog(ANALOG_RIGHT_X);
        bool L1 = controller.get_digital(DIGITAL_L1);
        bool L2 = controller.get_digital(DIGITAL_L2);
        bool R1 = controller.get_digital(DIGITAL_R1);
        bool R2 = controller.get_digital(DIGITAL_R2);
        bool btnU = controller.get_digital_new_press(DIGITAL_UP);
        bool btnD = controller.get_digital_new_press(DIGITAL_DOWN);
        bool btnA = controller.get_digital(DIGITAL_A);
        bool btnB = controller.get_digital(DIGITAL_B);
        bool btnX = controller.get_digital_new_press(DIGITAL_X);
        bool btnY = controller.get_digital_new_press(DIGITAL_Y);
        
        // 底盘锁
        if (overThreshold()) {
            motor_group_left.set_brake_mode(E_MOTOR_BRAKE_COAST);
            motor_group_right.set_brake_mode(E_MOTOR_BRAKE_COAST);
            chassisLock = false;
        }
        if (controller.get_digital(DIGITAL_L1)
            && controller.get_digital(DIGITAL_L2)
            && controller.get_digital(DIGITAL_R1)
            && controller.get_digital(DIGITAL_R2)) {
            chassisLock = true;
            chassis::move(0, 0);
        }
        if (chassisLock) {
            chassis::brake(pros::E_MOTOR_BRAKE_HOLD);
        } else {
            chassis::move(ch3 + ch1, ch3 - ch1);
        }		
        
        // 升降
        if (L1) {
            lift::lift_S(90);
        } else if (L2) {
            lift::lift_S(-70);
        } else {
            lift::lift_S(0);
        }

        // 滚轮
        if (R1) {
            motor_group_roller.move(127);
        } else {
            motor_group_roller.move(0);
        }

        // 夹子
        if (R2) {
            motor_clip.move(-127);
        } else {
            motor_clip.move(0);
        }

        if (btnA) {
            motor_toggle.move(127);
        } else {
            motor_toggle.move(0);
        }

        if (btnX) {
            pros::delay(1000);
            chassis::turnGyro(90, 5000);
        }
        if (btnY) {
            pros::delay(1000);
            chassis::turnGyro(45, 5000);
        }

		pros::delay(30);
	}
}
	

