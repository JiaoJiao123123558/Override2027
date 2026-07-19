#include "haws/auto.h"

using namespace chassis;
using namespace lift;

void redLeft1() {
    
    //撞杆两下
    digit_gun.set_value(true);
    delay(100);
    digit_gun.set_value(false);
    delay(200);
    chassis::moveEnc(20,300);
    digit_gun.set_value(false);
    delay(100);
    digit_gun.set_value(true);
    delay(100);
    chassis::moveEnc(-420,3000);//700
    lift::setGear(2); 
    delay(50);

    // 放预装
    
    turnGyro(-87,3000); 
    delay(50);
    motor_arm.move(50);
    delay(360);
    motor_arm.move(0);
    chassis::moveEnc(400,3000);
    lift::setGear(0);
    lift::lift(-100);
    delay(350);
    lift::lift(0);
    delay(20);
    digit_clip.set_value(true);
    delay(300);

    //后退取第一个桩夹住
    chassis::moveEnc(-320,3000);
    delay(200);
    turnGyro(-138,3000);
    delay(80);
    chassis::moveEnc(650,3000);
    turnGyro(-140,3000);
    delay(200);
    chassis::moveEnc(300,3000,20);
    delay(200);
    digit_clip.set_value(false);
    lift::setGear(1); 

    //夹住后抬升再往前走一点//转向并放置桩
    chassis::moveEnc(220,3000);
    delay(200);
    turnGyro(-3,3000);
    delay(100);
    lift::setGear(2); 
    delay(300);
    chassis::moveEnc(510,3000);
    delay(100);

    motor_arm.move(50);
    delay(130);
    motor_arm.move(0);
    delay(100);
    lift::setGear(1); 
    delay(500);
    motor_arm.move(50);
    delay(20);
    motor_arm.move(0);
    digit_clip.set_value(true);

    //取第二个桩
    chassis::moveEnc(-200,3000);
    lift::setGear(0); 
    turnGyro(-48,3000);
    motor_arm.move(-50);
    delay(50);
    motor_arm.move(0);
    chassis::moveEnc(580,3000);
    chassis::moveEnc(250,3000,20);
    delay(300);
    lift::lift(-100);
    delay(70);
    lift::lift(0);
    digit_clip.set_value(false);
    lift::setGear(3); 
    delay(100);
    turnGyro(50,3000);




    //chassis::moveEnc(1200,3000);
    
    //chassis::turnGyro(90,3000);*/
}

void redLeft2() {
  
}