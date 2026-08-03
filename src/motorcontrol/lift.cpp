#include "haws/motorcontrol/lift.h"
#include "haws/config.hpp"
#include "main.h"

namespace lift
{
void liftToTarget(void *armTask);
bool getTaskEnable();

Mutex liftGearMutex; // 互斥锁
int liftGear = 0;    // 当前档位
Mutex taskEnableMutex;   // 互斥锁
bool taskEnable = false; // 是否允许执行进程
Mutex targetPositionMutex; // 互斥锁
int targetPosition = 0;    // 摇臂抬升目标角度值

Task autoLift = Task(liftToTarget); // 切换档位进程

const int position[] = {
    5, 
    950,
    2550,
    5000,
    7740};

void lift(int power, motor_brake_mode_e brakeMode) {
    if (power == 0) {
        motor_group_lift.set_brake_mode_all(brakeMode);
        motor_group_lift.brake();
    } else {
        if (getTaskEnable()) {
            setTaskEnable(false);
            pros::delay(10);
            // autoLift.remove();
        }
        motor_group_lift.move(power * 1.27);
    }
}

void setTaskEnable(bool enable) {
    taskEnableMutex.take(100);
    taskEnable = enable;
    taskEnableMutex.give();
}

bool getTaskEnable() {
    taskEnableMutex.take(100);
    bool res = taskEnable;
    taskEnableMutex.give();
    return res;
}

int getGear() {
    liftGearMutex.take(100);
    int res = liftGear;
    liftGearMutex.give();
    return res;
}

void setGear(int gear) {
    // 只有在设置的档位与当前不同时有效
    if (gear != getGear()) {
        // 设置新的档位值
        pros::lcd::print(0, "set gear: %d -> %d", gear, position[gear]);    
        // logger::log("set gear: " + std::to_string(gear));
        liftGearMutex.take(100);
        liftGear = gear;
        liftGearMutex.give();
        // 若当前有线程正在运行, 释放资源
        if (getTaskEnable()) {
            setTaskEnable(false);
            pros::delay(10);
            // autoLift.remove();
        }
        // 开启新的线程执行换档
        setTaskEnable(true);
        pros::delay(10);
        // 获取档位对应的高度
        int curGear = getGear();
        liftToPosition(position[curGear]);
    }
}

void liftToTarget(void *armTask) {
    int target = targetPosition;
    uint32_t start_time = pros::millis();
    while (getTaskEnable()) {
        // 计算误差
        int err = target - sensor_lift.get_position();

        // 退出
        if (abs(err) < 50 || pros::millis() - start_time > 3000) {
            lift(0);
            setTaskEnable(false);
            break;
        }

        int power = CONSTRAIN(err * 0.1, -80, 80);
        if (power > 0) {
            power = CONSTRAIN(power, 27, 80);
        }
        if (power < 0) {
            power = CONSTRAIN(power, -80, -12);
        }

        motor_group_lift.move(power * 1.27);
        pros::delay(10);
    }
    pros::lcd::print(1, "liftToGear %d, %d/%d", getGear(), sensor_lift.get_position(), target);
}

void liftToPosition(int position, bool isAsync) {
    targetPositionMutex.take(100);
    targetPosition = position;
    targetPositionMutex.give();
    setTaskEnable(true);
    if (isAsync) {
        autoLift = Task(liftToTarget);
    } else {
        void *temp;
        liftToTarget(temp);
    }
}

}