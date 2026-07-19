#include "haws/auto.h"

using namespace chassis;
using namespace lift;

void redLeft1() {
    
    //撞杆两下
    digit_gun.set_value(true);
    delay(200);
    digit_gun.set_value(false);
    delay(200);
    chassis::moveEnc(20,300);
    digit_gun.set_value(true);
    delay(200);
    digit_gun.set_value(false);
    delay(200);
    chassis::moveEnc(-420,3000);//700
    lift::setGear(2); 

    // 放预装
    
    delay(100);
    turnGyro(-87,3000); 
    delay(100);
    motor_arm.move(50);
    delay(360);
    motor_arm.move(0);
    chassis::moveEnc(400,3000);
    lift::setGear(0);
    lift::lift(-100);
    delay(400);
    lift::lift(0);
    delay(200);
    digit_clip.set_value(true);
    delay(300);

    //后退取第一个桩夹住
    chassis::moveEnc(-320,3000);
    delay(200);
    turnGyro(-138,3000);
    delay(80);
    chassis::moveEnc(880,3000);
    delay(200);
    digit_clip.set_value(false);
    lift::setGear(1); 

    //夹住后抬升再往前走一点//转向并放置桩
    chassis::moveEnc(310,3000);
    delay(200);
    turnGyro(0,3000);
    delay(100);
    lift::setGear(2); 
    delay(300);
    chassis::moveEnc(440,3000);
    delay(50);

    // chassis::moveEnc(100,3000);
    // motor_arm.move(50);
    // delay(100);
    // motor_arm.move(0);
    // delay(500);

    // digit_clip.set_value(true);

    // //取第二个桩
    // chassis::moveEnc(-200,3000);
    // turnGyro(145,3000);
    // chassis::moveEnc(400,3000);


    //chassis::moveEnc(1200,3000);
    
    //chassis::turnGyro(90,3000);*/
}

void redLeft2() {
  
}