#include "main.h"
#include "haws/motorcontrol/chassis.h"

AIVision sensor_camera(0);

void findYellowPin() {    
    sensor_camera.reset();
    sensor_camera.enable_detection_types(pros::AivisionModeType::colors);

    pros::AIVision::Color yellow_sig = {
        .id = 1, 
        .red = 230, .green = 210, .blue = 40,   // High Red & Green, Low Blue creates Yellow
        .hue_range = 15,                        // Tight hue tolerance to filter ambient light
        .saturation_range = 0.6                 // Strong color saturation requirement
    };
    sensor_camera.set_color(yellow_sig);

    while (true) {
        auto objects = sensor_camera.get_all_objects();

        if (!objects.empty()) {
            auto& largest_object = objects[0];

            if (largest_object.id == 1) { // Confirms it is our yellow target
                int x_pos = largest_object.object.element.xoffset;
                int y_pos = largest_object.object.element.yoffset;
                int width = largest_object.object.element.width;
                int height = largest_object.object.element.height;
                pros::lcd::print(6, "target w: %d, h: %d", width, height);
                pros::lcd::print(7, "target x: %d, y: %d", x_pos, y_pos);
                int x_err = CONSTRAIN((x_pos - 120) * 0.3, -15, 15);
                if (abs(x_err) < 3) {
                    x_err = 0;
                }
                chassis::move(x_err, -x_err);
            } else {
                chassis::move(0, 0);
            }
        } else {
            chassis::move(0, 0);
        }
        pros::delay(20);
    }
}