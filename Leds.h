#ifndef Leds_h
#define Leds_h

#include <Arduino.h>

// Определение пинов для светодиодов (можно изменить)
#define RED_PIN 23
#define BLUE_PIN 18
#define GREEN_PIN 19

// Перечисление для удобства работы с цветами
enum LEDColor {
  RED,
  BLUE,
  GREEN
};

class Leds {
public:
  static void blink(LEDColor color1, unsigned long interval, int count = 0);
  static void blink(LEDColor color1, LEDColor color2, unsigned long interval, int count = 0);
  static void blink(LEDColor color1, LEDColor color2, LEDColor color3, unsigned long interval, int count = 0);
  static void on(LEDColor color, unsigned long duration = 0);
  static void off(LEDColor color = LEDColor::RED, bool all = true);
  static void tick();
  static void begin(); // Добавляем функцию инициализации
private:
  static int getColorPin(LEDColor color);
  static unsigned long blinkMillis;
  static int blinkState;
  static int blinkCount;
  static int blinkTotalCount;
  static LEDColor blinkColors[3];
  static int blinkColorCount;
  static unsigned long onMillis;
  static bool isOn;
  static LEDColor onColor;
  static unsigned long onDuration;
  static bool secondaryBlinkActive;
  static unsigned long secondaryBlinkMillis;
  static int secondaryBlinkCount;
  static int secondaryBlinkTotalCount;
  static LEDColor secondaryBlinkColor;
  static unsigned long secondaryBlinkInterval;
  static bool mainBlinkActive; // Добавляем флаг для основного мигания
  static bool secondaryBlinkInterruptMain; // Добавляем флаг для прерывания основного мигания
  static bool mainBlinkPaused; // Добавляем флаг для паузы основного мигания
  static unsigned long mainBlinkPausedMillis; // Добавляем переменную для времени паузы основного мигания
  static LEDColor mainBlinkColor; // Добавляем переменную для основного цвета мигания.
};

#endif