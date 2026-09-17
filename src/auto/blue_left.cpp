#include "haws/auto.h"
#include "haws/motorcontrol/chassis.h"

using namespace chassis;
using namespace lift;

void blueLeft1() {
    turnGyro(90,3000);

}

void blueLeft2() {
    // rushGyro(-20, TURN_MIN_V);
    lift::liftToPosition(3000, false);
    pros::delay(3000);
    lift::liftToPosition(5000, false);
    pros::delay(3000);
    lift::liftToPosition(2000);
}