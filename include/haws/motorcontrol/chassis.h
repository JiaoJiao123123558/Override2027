#ifndef _HAWS_CHASSIS_H_
#define _HAWS_CHASSIS_H_

#include "api.h"

using namespace pros;

namespace chassis {
// 直行
#define STRIGHT_ACC 8
#define STRIGHT_MIN_V 20
// 转向
#define TURN_ACC 20
#define TURN_MIN_V 20

/**
 * @brief 重置所有底盘电机编码器
 */
void reset();

/**
 * @brief 底盘控制基础函数
 * @param lPower 左边电机的功率值(-100至100)
 * @param rPower 右边电机的功率值(-100至100)
 */
void move(int lPower, int rPower);

/**
 * @brief 底盘刹车函数
 * @param brakeMode 拓展参数: 刹车模式(默认为brake)
 * @note 如不填入刹车模式, 使用brake模式进行刹车
 */
void brake(motor_brake_mode_e brakeMode = E_MOTOR_BRAKE_BRAKE);

/**
 * @brief 判断底盘是否卡死
 * @return 判断结果
 */
bool isChassisStack();

/**
 * @brief 读取底盘编码器的值
 * @return 底盘所有编码器的平均值
 */
int getPosition();

/**
 * @brief 分别读取底盘编码器的值
 * @param leftPos 引用参数，通过参数返回左侧所有电机 {编码器的平均值}
 * @param leftPos 引用参数，通过参数返回右侧所有电机 {编码器的平均值}
 * @return 底盘编码器的值
 */
void getPositionSeparate(int &leftPos, int &rightPos);

/**
 * @brief 编码器控制底盘直行函数
 * @param distance  移动的距离(电机的编码器目标值, 正数为前进, 负数为后退)
 */
void moveEnc(int distance);

/**
 * @brief 使用陀螺仪控制底盘转向函数
 * @param angle     底盘朝向的角度值(陀螺仪相对初始位置的值)
 */
void turnGyro(float angle);
}
#endif
