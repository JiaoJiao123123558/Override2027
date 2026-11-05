#include "haws/motorcontrol/chassis.h"
#include "haws/config.hpp"
#include <cmath>
#include "haws/display/canvas.h"
#include "pros/llemu.hpp"

namespace chassis
{

void reset() {
    motor_group_left.tare_position_all();
    motor_group_right.tare_position_all();
}

void move(int lPower, int rPower) {
    motor_group_left.move(lPower * 1.27);
    motor_group_right.move(rPower * 1.27);
}

void brake(motor_brake_mode_e brakeMode) {
    motor_group_left.set_brake_mode_all(brakeMode);
    motor_group_right.set_brake_mode_all(brakeMode);
    motor_group_left.brake();
    motor_group_right.brake();
}

bool isChassisStack() {
    return (motor_group_left.get_actual_velocity() < STRIGHT_MIN_V && 
            motor_group_right.get_actual_velocity() < STRIGHT_MIN_V);
}

int getPosition() {
    std::vector<double> leftPositions = motor_group_left.get_position_all();
    std::vector<double> rightPositions = motor_group_right.get_position_all();

    int total = 0;
    int count = 0;
    for (int leftPos : leftPositions) {
        total += leftPos;
        count += 1;
    }
    for (int rightPos : rightPositions) {
        total += rightPos;
        count += 1;
    }
    return total / count;
}

void getPositionSeparate(int &leftPos, int &rightPos) {
    std::vector<double> leftPositions = motor_group_left.get_position_all();
    std::vector<double> rightPositions = motor_group_right.get_position_all();
    int leftTotal = 0, rightTotal = 0;
    int leftCount = 0, rightCount = 0;

    for (int pos : leftPositions) {
        leftTotal += pos;
        leftCount += 1;
    }
    leftPos = leftTotal / leftCount;

    for (int pos : rightPositions) {
        rightTotal += pos;
        rightCount += 1;
    }
    rightPos = rightTotal / rightCount;
}

void moveEnc(int distance) {
    int current = 0;
    int sign = 1;
    float power = 0;
    bool isBrake = false, isStack = false;
    // 记录符号，控制输出都以第一象限计算
    if (distance < 0) {
        sign = -1;
        distance = -distance;
    }

    // 测试用, 绘图
    Canvas canvas;
    canvas.setTarget(distance);
    
    reset();
    uint32_t current_time = 0;
    uint32_t stack_time = 0;
    uint32_t brake_time = 0;
    while (true) {
        // 更新当前编码器值
        current = abs(getPosition());
        current_time = pros::millis();

        // 电机卡顿判断
        if (current_time > 1000) {
            if (isChassisStack()) {
                if (!isStack) {
                    isStack = true;
                    stack_time = pros::millis();
                }
            } else {
                isStack = false;
            }
        }

        // 退出条件判断
        if (isBrake && current_time - brake_time > 200) {
            break;
        }
        if (isStack && current_time - stack_time > 500) {
            break;
        }

        // 计算输出功率
        if (current < distance / 2) {
            power = STRIGHT_MIN_V + fabs(sqrt(current * STRIGHT_ACC));
        } else if (current < distance) {
            power = fabs(sqrt((distance - current) * STRIGHT_ACC));
        } else {
            if (isBrake == false) {
                brake_time = pros::millis();
                isBrake = true;
            }
            power = 0;
        }
        power = CONSTRAIN(power, 0, 100);

        // 底盘输出
        if (power == 0) {
            brake();
        } else {
            move(power * sign, power * sign);
        }

        // 测试用, 绘图
        canvas.addData(current, power);

        pros::delay(10);
    }
    pros::lcd::print(0, "%d", getPosition());
}

void turnGyro(float angle) {
    float current = 0.0;
    int sign = 1;
    float power = 0;
    float start_angle = sensor_gyro.get_rotation();
    float target = (angle - start_angle);
    bool isBrake = false, isStack = false;
    // 记录符号，控制输出都以第一象限计算
    if (target < 0) {
        sign = -1;
        target = -target;
    }

    // 测试用, 绘图
    Canvas canvas;
    canvas.setTarget(target);

    reset();
    uint32_t current_time = 0;
    uint32_t stack_time = 0;
    uint32_t brake_time = 0;
    while (true) {
        // 更新当前编码器值
        current = fabs(sensor_gyro.get_rotation() - start_angle);
        current_time = pros::millis();
        
        // 电机卡顿判断
        if (current_time > 1000) {
            if (isChassisStack()) {
                if (!isStack) {
                    isStack = true;
                    stack_time = pros::millis();
                }
            } else {
                isStack = false;
            }
        }

        // 退出条件判断
        if (isBrake && current_time - brake_time > 200) {
            break;
        }
        if (isStack && current_time - stack_time > 500) {
            break;
        }

        // 计算输出功率
        if (current < target / 2) {
            power = TURN_MIN_V + fabs(sqrt(current * TURN_ACC));
        } else if (current < target) {
            power = fabs(sqrt((target - current) * TURN_ACC));
        } else {
            if (isBrake == false) {
                brake_time = pros::millis();
                isBrake = true;
            }
            power = 0;
        }
        power = CONSTRAIN(power, 0, 100);

        // 底盘输出
        if (power == 0) {
            brake();
        } else {
            move(power * sign, - power * sign);
        }

        // 测试用, 绘图
        canvas.addData(current, power);

        pros::delay(10);
    }
    pros::lcd::print(0, "%.2f", sensor_gyro.get_rotation());
}


}