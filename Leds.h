#ifndef Leds_h
#define Leds_h

#include <Arduino.h>
#include <initializer_list>

#define RED_LED_PIN 23
#define BLUE_LED_PIN 18
#define GREEN_LED_PIN 19
#define DEFAULT_INTERVAL 500;
#define SIZE_COLORS 10

enum LEDColor {
    RED,
    BLUE,
    GREEN
};

struct BlinkConfig {
    unsigned long millis = 0;
    int state = 0;
    int count = 0;
    int totalCount = 0;
    LEDColor colors[SIZE_COLORS];
    unsigned long interval = DEFAULT_INTERVAL;
    int colorCount = 0;
    bool active = false;
};

class Leds {
public:
    static void blink(std::initializer_list<LEDColor> colors, int count = 0,
                      unsigned long interval = mainBlink.interval);
    static void blinkOff();
    static void on(LEDColor color, unsigned long duration = 0);
    static void off();
    static void tick();
    static void setInterval(unsigned long time);
    static void begin(int redPin, int greenPin, int bluePin);
    static void setOnLevel(int level);

private:
    static int getColorPin(LEDColor color);
    static void turnOffAllLedPins();
    static void setLedState(LEDColor color, bool state);
    static void handleBlink(BlinkConfig &blinkConfig);
    static bool hasElapsed(unsigned long startMillis, unsigned long duration);
    static unsigned long currentMillis;
    static unsigned long onMillis;
    static bool isOn;
    static bool isOnPrev;
    static LEDColor onColor;
    static unsigned long onDuration;
    static bool secondaryBlinkInterruptMain;
    static bool mainBlinkPaused;
    static unsigned long mainBlinkPausedMillis;
    static int redPin;
    static int bluePin;
    static int greenPin;
    static int ledOnLevel;
    static BlinkConfig mainBlink;
    static BlinkConfig secondaryBlink;
};

#endif
