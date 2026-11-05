#include "haws/display/canvas.h"
#include "api.h"

Canvas::Canvas() {
    draw_amplifier = 1;
    lastX = 0;
    lastPower = 0;
    lastCurrent = 0;
    clear();
}

Canvas::~Canvas() {

}

void Canvas::clear() {
    pros::screen::set_eraser(pros::Color::black);
    pros::screen::erase();
    pros::screen::set_pen(pros::Color::white);
    // pros::screen::draw_line(0, SCREEN_HEIGHT,
    //                         SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Canvas::setTarget(float target) {
    draw_amplifier = (SCREEN_HEIGHT - 30) / target;
    pros::screen::draw_line(
        0, 30,
        SCREEN_WIDTH, 30);
}

void Canvas::addData(float current, float power) {
    float newX = lastX + 3;
    float newCurrent = current * draw_amplifier;
    float newPower = power * SCREEN_HEIGHT / 100.0;

    pros::screen::set_pen(pros::Color::yellow);
    pros::screen::draw_line(
        lastX, SCREEN_HEIGHT - lastPower,
        newX, SCREEN_HEIGHT - newPower);
    pros::screen::set_pen(pros::Color::blue);
    pros::screen::draw_line(
        lastX, SCREEN_HEIGHT - lastCurrent,
        newX, SCREEN_HEIGHT - newCurrent);
    
    lastX = newX;
    lastPower = newPower;
    lastCurrent = newCurrent;
}