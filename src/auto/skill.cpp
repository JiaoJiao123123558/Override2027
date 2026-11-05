#include "haws/auto.h"

void skill() {
    sensor_lift.reset_position();


    chassis::moveEnc(700);
    lift::liftToPosition(1100);
    chassis::turnGyro(83);
    chassis::moveEnc(430);
}
