
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 230

class Canvas {
public:
    Canvas();
    ~Canvas();

    void clear();

    void setTarget(float target);

    void addData(float current, float power);

private:
    float draw_amplifier;

    float lastX;
    float lastCurrent;
    float lastPower;
};