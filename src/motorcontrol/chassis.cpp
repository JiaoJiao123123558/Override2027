#include "haws/motorcontrol/chassis.h"
#include "haws/config.hpp"
#include "haws/display/logger.h"
#include "pros/colors.hpp"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "haws/motorcontrol/pid.h"
#include "pros/rtos.hpp"
#include "pros/screen.hpp"
#include <cmath>
#include <cstdint>

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

void moveEnc(int distance, int timeout, int maxPower, motor_brake_mode_e brakeMode) {
    int current = 0;
    int sign = 1;
    float power = 0;
    uint32_t start_time = pros::millis();
    // 记录符号，控制输出都以第一象限计算
    if (distance < 0) {
        sign = -1;
        distance = -distance;
    }

    reset();
    while (true) {
        // 更新当前编码器值
        current = abs(getPosition());

        // 退出条件判断
        if (distance - current <= 20 || pros::millis() - start_time > timeout) {
            break;
        }

        // 计算输出功率
        if (current < distance / 2) {
            power = STRIGHT_MIN_V + fabs(sqrt(current * 2 * STRIGHT_ACC));
        } else {
            power = fabs(sqrt((distance - current) * 2 * STRIGHT_ACC));
        }
        power = CONSTRAIN(power, STRIGHT_MIN_V, maxPower);

        // 底盘输出
        move(power * sign, power * sign);

        pros::delay(15);
    }

    if (brakeMode != pros::E_MOTOR_BRAKE_INVALID) {
        brake(brakeMode);
    }
    Logger::getInstance().info("moveEnc: [%d, %d]", current, distance);
}

void moveEnc_S(int distance, int maxPower) {
    int current = 0;
    int sign = 1;
    float power = 0;
    bool isStack = false;
    uint32_t start_time = pros::millis();
    uint32_t stack_time = 0;
    uint32_t current_time = 0;
    // 记录符号，控制输出都以第一象限计算
    if (distance < 0) {
        sign = -1;
        distance = -distance;
    }

    // Draw the baseline.
    double draw_amplifier = 230.0 / abs(100);
    pros::screen::set_eraser(Color::black);
    pros::screen::erase();
    pros::screen::set_pen(Color::white);
    pros::screen::draw_line(0, abs(100) * draw_amplifier,
                            600, abs(100) * draw_amplifier);
    pros::screen::set_pen(Color::blue);
    double last = 0; // for draw line
    int index = 1; // for draw line

    reset();
    while (true) {
        // 更新当前编码器值
        current = abs(getPosition());
        current_time = pros::millis();

        // 退出条件判断
        if (distance - current <= 20) {
            break;
        }

        // 计算输出功率
        int err = distance - current;
        power = CONSTRAIN(err * STRIGHT_S_DCC, STRIGHT_MIN_V, maxPower);
        if (current_time - start_time < STRIGHT_S_ACC_TIME) {
            power = power * current_time / STRIGHT_S_ACC_TIME;
        }
        // 底盘输出
        // Logger::getInstance().info("%d", power);
        move(power * sign, power * sign);

        // Draw line
        pros::screen::draw_line(
            index * 3, fabs(last) * draw_amplifier,
            (index + 1) * 3, fabs(power * draw_amplifier));
        last = power;
        index++;

        // 电机卡顿判断
        if (current_time > STRIGHT_S_ACC_TIME) {
            if (motor_group_left.get_actual_velocity() < 5 &&
                motor_group_right.get_actual_velocity() < 5) {
                if (!isStack) {
                    isStack = true;
                    stack_time = pros::millis();
                }
            } else {
                isStack = false;
            }
        }
        if (current_time - stack_time > 3000) {
            break;
        }

        pros::delay(10);
    }
    brake(E_MOTOR_BRAKE_BRAKE);
    pros::delay(200);
    pros::lcd::print(7, "moveEnc: [%d / %d]", getPosition(), distance);
}

void turnGyroPID(int angle, int timeout, int maxPower) {
    double kp = 1.0;
    double ki = 0.03; // 0.03
    double kd = 1.0; // 4.3
    PID pid = PID(kp, ki, kd);

    pid.SetTarget(angle);
    pid.SetIntegralMax(10);
    pid.SetIntegralRange(10);

    pid.SetSmallBigErrorTolerance(1, 3);
    pid.SetSmallBigErrorDuration(50, 250);
    pid.SetDerivativeTolerance(4.5);

    // Draw the baseline.
    double draw_amplifier = 230.0 / abs(angle);
    pros::screen::set_eraser(Color::black);
    pros::screen::erase();
    pros::screen::set_pen(Color::white);
    pros::screen::draw_line(0, abs(angle) * draw_amplifier,
                            600, abs(angle) * draw_amplifier);
    pros::screen::set_pen(Color::blue);

    // Start the PID loop.
    uint32_t start_time = pros::millis();
    double output;
    double current;
    double last = 0;
    int index = 1; // for draw line
    while (true) {
        // 更新当前陀螺仪值
        current = sensor_gyro.get_rotation();

        // 退出条件判断
        if (pid.TargetArrived() || pros::millis() - start_time > timeout) {
            break;
        }

        // 计算输出功率
        output = pid.Update(current);
        last = current;

        // Draw line
        pros::screen::draw_line(
            index * 3, fabs(last) * draw_amplifier,
            (index + 1) * 3, fabs(current * draw_amplifier));
        index++;
        
        // 底盘输出
        move(CONSTRAIN(output, -maxPower, maxPower),
             CONSTRAIN(-output, -maxPower, maxPower));

        pros::delay(10);
    }
    brake(pros::E_MOTOR_BRAKE_HOLD);
    pros::lcd::print(7, "turnGyroPID: [%.2f / %d] %dms", current, angle, pros::millis() - start_time);
}

void turnGyro(float angle, int timeout, int maxPower, motor_brake_mode_e brakeMode) {
    float current = 0.0;
    int sign = 1;
    float power = 0;
    uint32_t start_time = pros::millis();
    float start_angle = sensor_gyro.get_rotation();
    float target = (angle - start_angle) * 10.0;
    // 记录符号，控制输出都以第一象限计算
    if (target < 0) {
        sign = -1;
        target = -target;
    }

    reset();
    while (true) {
        // 更新当前编码器值
        current = fabs(sensor_gyro.get_rotation() - start_angle) * 10;

        // 退出条件判断（小于3度）
        if (target - current <= 30 || pros::millis() - start_time > timeout) {
            break;
        }

        // 计算输出功率
        if (current < target / 2) {
            power = TURN_MIN_V + fabs(sqrt(current * 2 * TURN_ACC));
        } else {
            power = fabs(sqrt((target - current) * 2 * TURN_DCC));
        }
        power = CONSTRAIN(power, TURN_MIN_V, maxPower);

        // 底盘输出
        move(power * sign, - power * sign);

        pros::delay(15);
    }

    if (brakeMode != pros::E_MOTOR_BRAKE_INVALID) {
        brake(brakeMode);
    }
    Logger::getInstance().info("turnGyro: [%f / %f]", sensor_gyro.get_rotation(), angle);
}

void rushGyro(int angle, int power) {
    float err = angle - sensor_gyro.get_rotation();
    int timer = 0;
    if (err > 0) {
        while (err > 3) {
            chassis::move(power, -power);
            pros::delay(15);
            timer += 15;
            err = angle - sensor_gyro.get_rotation();
        }
    } else {
        while (err < -3) {
            chassis::move(-power, power);
            pros::delay(15);
            timer += 15;
            err = angle - sensor_gyro.get_rotation();
        }
    }
    chassis::move(0, 0);
    chassis::brake();
    pros::delay(10);
    // Logger::getInstance().info("rushGyo: [%f / %d], time=%Ds", sensor_gyro.get_rotation(), angle, double (timer / 1000.0));
    Logger::getInstance().info("rushGyro: [%f / %d]", sensor_gyro.get_rotation(), angle);
}

// void findObject(int angleRange) {
//     int minDistance = sensor_dis.get_distance();
//     float currentAngle = sensor_gyro.get_rotation();
//     float minDistanceAngle = currentAngle;
//     rushGyro(currentAngle - angleRange,TURN_MIN_V);
//     while (true) {
//         if (sensor_gyro.get_rotation() > currentAngle + angleRange) {
//             break;
//         }

//         move(TURN_MIN_V, -TURN_MIN_V);

//         int currentDistance = sensor_dis.get_distance();
//         if (currentDistance < minDistance) {
//             minDistance = currentDistance;
//             minDistanceAngle = sensor_gyro.get_rotation();
//             Logger::getInstance().info("update Angle: dis: %d, %f", minDistance, minDistanceAngle);
//         }

//         pros::delay(10);
//     }

//     rushGyro(minDistanceAngle - 1, TURN_MIN_V);
// }

void rushDisFront(int distance, int power) {
    float err = sensor_disFront.get_distance() - distance;
    int timer = 0;
    chassis::reset();
    if (err > 0) {
        while (err > 10) {
            chassis::move(power, power);
            pros::delay(15);
            timer += 15;
            err = sensor_disFront.get_distance() - distance;
        }
    } else {
        while (err < -10) {
            chassis::move(-power, -power);
            pros::delay(15);
            timer += 15;
            err = sensor_disFront.get_distance() - distance;
        }
    }
    chassis::move(0, 0);
    chassis::brake();
    pros::delay(10);
    Logger::getInstance().info("rushDisFront: [%d / %d]", sensor_disFront.get_distance(), distance);
}

void rushDisBack(int distance, int power) {
    float err = sensor_disBack.get_distance() - distance;
    int timer = 0;
    chassis::reset();
    if (err > 0) {
        while (err > 10) {
            chassis::move(-power, -power);
            pros::delay(15);
            timer += 15;
            err = sensor_disBack.get_distance() - distance;
        }
    } else {
        while (err < -10) {
            chassis::move(power, power);
            pros::delay(15);
            timer += 15;
            err = sensor_disBack.get_distance() - distance;
        }
    }
    chassis::move(0, 0);
    chassis::brake();
    pros::delay(10);
    Logger::getInstance().info("rushDisBack: [%d / %d]", sensor_disBack.get_distance(), distance);
}

void moveDis_t(DistancePos pos, int distance, int timeout, int maxPower, motor_brake_mode_e brakeMode) {
    float err = 0.0, lastErr = 0.0, totalErr = 0.0, deltaErr = 0.0; // 用于计算直行pid
    float power = 0.0; // 输出功率
    if (pos == front) {
        err = sensor_disFront.get_distance() - distance;
    } else {
        err = sensor_disBack.get_distance() - distance;
        Logger::getInstance().info("err: %.2f = %d - %d", err, sensor_disBack.get_distance(), distance);
    }
    
    if (fabs(err) < 30) {
        if (pos == front) {
            rushDisFront(distance, 30);
        } else {
            rushDisBack(distance, 30);
        }
        return;
    }

    float kp = 0.28, ki = 0.01, kd = 1.0;
    // 计时器
    int timer = 0; // 用于超时退出循环，未使用计时器计时，timer每次循环+15
    
    chassis::reset();
    while (true) {
        // 计算直行pid
        if (pos == front) {
            err = sensor_disFront.get_distance() - distance;
        } else {
            err = sensor_disBack.get_distance() - distance;
        }
        
        // 控制积分值在1以下，且分段积分
        if (fabs(err) < 50) {
            totalErr += err;
        }
        deltaErr = err - lastErr;
        lastErr = err;
        // 判据为转向超过目标值，需要反转的情况
        if (err * lastErr <= 0) {
            totalErr = 0;
        }
        power = kp * err + ki * totalErr + kd * deltaErr;
        // 缓加速
        power = power * CONSTRAIN(timer, 0, 210) / 210.0;
        if (pos == back) {
            power = -power;
        }
        // 退出条件
        if (fabs(err) < 15 || timer > timeout) {
            break;
        }
        // TODO: 确认是否需要在超限时再累计一个power_gyro
        chassis::move(CONSTRAIN(power, -maxPower, maxPower),
                      CONSTRAIN(power, -maxPower, maxPower));
        // 分段积分
        if (fabs(err) > abs(distance) * 0.2 || fabs(err) < 1) {
            totalErr = 0;
        }
        // 延时
        pros::delay(15);
        timer += 15;
    }
    chassis::move(0, 0);
    chassis::brake(brakeMode);
    pros::delay(10);
    Logger::getInstance().info("moveDis: [%d / %d]", sensor_disFront.get_distance(), distance);
}

void moveDis(DistancePos pos, int distance, int timeout, int maxPower, motor_brake_mode_e brakeMode) {
    int current = 0, start = 0;
    int sign = 1;
    float power = 0;
    if (pos == front) {
        start = sensor_disFront.get_distance();
    } else {
        start = sensor_disBack.get_distance();
    }
    // 记录符号，控制输出都以第一象限计算
    if (pos == front && start < distance) {
        sign = -1;
    }
    if (pos == back && start > distance) {
        sign = -1;
    }

    reset();
    uint32_t start_time = pros::millis();
    while (true) {
        // 更新当前编码器值
        if (pos == front) {
            current = sensor_disFront.get_distance();
        } else {
            current = sensor_disBack.get_distance();
        }

        // 退出条件判断
        if ((start < distance && distance - current < 10) ||
            (start > distance && current - distance < 10) ||
            pros::millis() - start_time > timeout) {
            break;
        }

        // 计算输出功率
        if (start < distance) {
            if (current < (start + distance) / 2) {
                power = STRIGHT_MIN_V + 10 + fabs(sqrt((current - start) * 2 * STRIGHT_ACC));
            } else {
                power = fabs(sqrt((distance - current) * 2 * STRIGHT_ACC));
            }
        } else {
            if (current > (start + distance) / 2) {
                power = STRIGHT_MIN_V + 10 + fabs(sqrt((start - current) * 2 * STRIGHT_ACC));
            } else {
                power = fabs(sqrt((current - distance) * 2 * STRIGHT_ACC));
            }
        }
        power = CONSTRAIN(power, STRIGHT_MIN_V, maxPower);

        // 底盘输出
        move(power * sign, power * sign);

        pros::delay(15);
    }

    if (brakeMode != pros::E_MOTOR_BRAKE_INVALID) {
        brake(brakeMode);
    }
    if (pos == front) {
        Logger::getInstance().info("moveDisF: [%d / %d]", sensor_disFront.get_distance(), distance);
    } else {
        Logger::getInstance().info("moveDisB: [%d / %d]", sensor_disBack.get_distance(), distance);
    }
}

}