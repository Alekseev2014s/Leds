#include "Leds.h"

unsigned long Leds::blinkMillis = 0;
int Leds::blinkCount = 0;
int Leds::blinkTotalCount = 0;
LEDColor Leds::blinkColors[3] = {LEDColor::RED, LEDColor::BLUE, LEDColor::GREEN};
unsigned long Leds::blinkInterval = 0;
int Leds::blinkColorCount = 0;
unsigned long Leds::onMillis = 0;
bool Leds::isOn = false;
LEDColor Leds::onColor = LEDColor::RED;
unsigned long Leds::onDuration = 0;
bool Leds::secondaryBlinkActive = false;
unsigned long Leds::secondaryBlinkMillis = 0;
int Leds::secondaryBlinkCount = 0;
int Leds::secondaryBlinkTotalCount = 0;
LEDColor Leds::secondaryBlinkColor[3] = {LEDColor::RED, LEDColor::BLUE, LEDColor::GREEN};
unsigned long Leds::secondaryBlinkInterval = 0;
int Leds::secondaryBlinkColorCount = 0;
bool Leds::mainBlinkActive = false;
bool Leds::secondaryBlinkInterruptMain = false;
bool Leds::mainBlinkPaused = false;
unsigned long Leds::mainBlinkPausedMillis = 0;
LEDColor Leds::mainBlinkColor = LEDColor::RED;

void Leds::begin() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

void Leds::blink(LEDColor color1, unsigned long interval, int count) {
  secondaryBlinkActive = count > 0;
  secondaryBlinkColor[0] = color1;
  secondaryBlinkInterval = interval;
  secondaryBlinkColorCount = 1;
  secondaryBlinkCount = 0;
  secondaryBlinkTotalCount = count;
  secondaryBlinkInterruptMain = secondaryBlinkActive;
  mainBlinkPaused = secondaryBlinkActive;
  mainBlinkPausedMillis = millis();

  if (!secondaryBlinkActive) {
    blinkColors[0] = color1;
    blinkInterval = interval;
    blinkColorCount = 1;
    blinkMillis = millis();
    blinkCount = 0;
    mainBlinkActive = true;
    mainBlinkColor = color1;
  }

  digitalWrite(RED_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

void Leds::blink(LEDColor color1, LEDColor color2, unsigned long interval, int count) {
  secondaryBlinkActive = count > 0;
  secondaryBlinkColor[0] = color1;
  secondaryBlinkColor[1] = color2;
  secondaryBlinkInterval = interval;
  secondaryBlinkColorCount = 2;
  secondaryBlinkCount = 0;
  secondaryBlinkTotalCount = count;
  secondaryBlinkInterruptMain = secondaryBlinkActive;
  mainBlinkPaused = secondaryBlinkActive;
  mainBlinkPausedMillis = millis();

  if (!secondaryBlinkActive) {
    blinkColors[0] = color1;
    blinkColors[1] = color2;
    blinkInterval = interval;
    blinkColorCount = 2;
    blinkTotalCount = 0;
    blinkMillis = millis();
    blinkCount = 0;
    mainBlinkActive = true;
    mainBlinkColor = color1;
  }

  digitalWrite(RED_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

void Leds::blink(LEDColor color1, LEDColor color2, LEDColor color3, unsigned long interval, int count) {
  secondaryBlinkActive = count > 0;
  secondaryBlinkColor[0] = color1;
  secondaryBlinkColor[1] = color2;
  secondaryBlinkColor[2] = color3;
  secondaryBlinkInterval = interval;
  secondaryBlinkColorCount = 3;
  secondaryBlinkCount = 0;
  secondaryBlinkTotalCount = count;
  secondaryBlinkInterruptMain = secondaryBlinkActive;
  mainBlinkPaused = secondaryBlinkActive;
  mainBlinkPausedMillis = millis();

  if (!secondaryBlinkActive) {
    blinkColors[0] = color1;
    blinkColors[1] = color2;
    blinkColors[2] = color3;
    blinkInterval = interval;
    blinkColorCount = 3;
    blinkTotalCount = 0;
    blinkMillis = millis();
    blinkCount = 0;
    mainBlinkActive = true;
    mainBlinkColor = color1;
  }

  digitalWrite(RED_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

void Leds::on(LEDColor color, unsigned long duration) {
  onColor = color;
  onDuration = duration;
  isOn = true;
  onMillis = millis();
  digitalWrite(getColorPin(color), HIGH);
  secondaryBlinkActive = false;
  mainBlinkActive = false;
  secondaryBlinkInterruptMain = false;
  mainBlinkPaused = false;
}

void Leds::off(LEDColor color, bool all) {
  if (all) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    blinkColorCount = 0;
    secondaryBlinkActive = false;
    isOn = false;
    mainBlinkActive = false;
    secondaryBlinkInterruptMain = false;
    mainBlinkPaused = false;
  } else {
    digitalWrite(getColorPin(color), LOW);
  }
}

int Leds::getColorPin(LEDColor color) {
  switch (color) {
    case RED:
      return RED_PIN;
    case BLUE:
      return BLUE_PIN;
    case GREEN:
      return GREEN_PIN;
    default:
      return RED_PIN;
  }
}

void Leds::tick() {
  const unsigned long currentMillis = millis();

  // Обработка основного мигания
  if (mainBlinkActive && blinkColorCount > 0 && !secondaryBlinkInterruptMain && !mainBlinkPaused) {
    if (currentMillis - blinkMillis >= blinkInterval) {
      blinkMillis = currentMillis;

      const int colorIndex = blinkCount % blinkColorCount;

      digitalWrite(getColorPin(blinkColors[colorIndex]), !digitalRead(getColorPin(blinkColors[colorIndex])));

      if (digitalRead(getColorPin(blinkColors[colorIndex])) == LOW) {
        blinkCount++;
      }

      // Проверяем, достигнуто ли заданное количество миганий
      if (blinkTotalCount > 0 && blinkCount >= blinkTotalCount) {
        // Останавливаем мигание после достижения заданного количества
        mainBlinkActive = false;
        off(RED, true); // Выключаем все светодиоды
        blinkCount = 0;
        blinkTotalCount = 0;
      } else {
        // Переходим к следующему цвету
        if (blinkCount >= blinkColorCount) {
          blinkCount = 0; // Сбрасываем индекс цвета
        }
      }
    }
  }

  // Обработка включения
  if (isOn && onDuration > 0 && currentMillis - onMillis >= onDuration) {
    digitalWrite(getColorPin(onColor), LOW);
    isOn = false;
  }

  // Обработка вторичного мигания
  if (secondaryBlinkActive && currentMillis - secondaryBlinkMillis >= secondaryBlinkInterval) {
    secondaryBlinkMillis = currentMillis;

    int colorIndex = secondaryBlinkCount % secondaryBlinkColorCount;

    Serial.print("Secondary blink, colorIndex: ");
    Serial.println(colorIndex);

    Serial.print("read pin: ");
    Serial.println(digitalRead(getColorPin(secondaryBlinkColor[colorIndex])));

    digitalWrite(getColorPin(secondaryBlinkColor[colorIndex]), !digitalRead(getColorPin(secondaryBlinkColor[colorIndex])));

    if (digitalRead(getColorPin(secondaryBlinkColor[colorIndex])) == LOW) {
      secondaryBlinkCount++;
    }

    // Проверяем, достигнуто ли заданное количество МИГАНИЙ
    if (secondaryBlinkTotalCount > 0 && secondaryBlinkCount >= secondaryBlinkTotalCount) {
      secondaryBlinkActive = false;
      secondaryBlinkCount = 0;
      secondaryBlinkInterruptMain = false;
      digitalWrite(RED_PIN, LOW);
      digitalWrite(BLUE_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
    }
  }

  // Обработка паузы основного мигания
  if (mainBlinkPaused && currentMillis - mainBlinkPausedMillis >= secondaryBlinkInterval) {
    mainBlinkPaused = false;
  }
}
