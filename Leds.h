#ifndef Leds_h
#define Leds_h

#include <Arduino.h>
#include <initializer_list>

#define RED_LED_PIN 23
#define BLUE_LED_PIN 18
#define GREEN_LED_PIN 19

enum LEDColor {
  RED,
  BLUE,
  GREEN
};

class Leds {
public:
  static void blink(std::initializer_list<LEDColor> colors, unsigned long interval = blinkInterval, int count = 0);
  static void on(LEDColor color, unsigned long duration = 0);
  static void off(LEDColor color = LEDColor::RED, bool all = true);
  static void tick();
  static void setInterval(unsigned long time);
  static void begin(int redPin, int greenPin, int bluePin);
private:
  static int getColorPin(LEDColor color);
  static int redPin;
  static int bluePin;
  static int greenPin;
  static unsigned long blinkMillis;
  static int blinkState;
  static int blinkCount;
  static int blinkTotalCount;
  static LEDColor blinkColors[3];
  static unsigned long blinkInterval;
  static int blinkColorCount;
  static unsigned long onMillis;
  static bool isOn;
  static LEDColor onColor;
  static unsigned long onDuration;
  static bool secondaryBlinkActive;
  static unsigned long secondaryBlinkMillis;
  static int secondaryBlinkCount;
  static int secondaryBlinkTotalCount;
  static LEDColor secondaryBlinkColor[3];
  static unsigned long secondaryBlinkInterval;
  static int secondaryBlinkColorCount;
  static bool mainBlinkActive;
  static bool secondaryBlinkInterruptMain;
  static bool mainBlinkPaused;
  static unsigned long mainBlinkPausedMillis;
  static LEDColor mainBlinkColor;
};

#endif