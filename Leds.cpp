#include "Leds.h"

// Инициализация статических переменных
unsigned long Leds::blinkMillis = 0;
int Leds::blinkState = LOW;
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
bool Leds::mainBlinkActive = false; // Инициализируем флаг основного мигания
bool Leds::secondaryBlinkInterruptMain = false; // Инициализируем флаг прерывания основного мигания
bool Leds::mainBlinkPaused = false; // Инициализируем флаг паузы основного мигания
unsigned long Leds::mainBlinkPausedMillis = 0; // Инициализируем переменную времени паузы основного мигания
LEDColor Leds::mainBlinkColor = LEDColor::RED; // Инициализируем переменную основного цвета мигания

void Leds::begin() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

// Функция для мигания одним светодиодом
void Leds::blink(LEDColor color1, unsigned long interval, int count) {
  if (count > 0) {
    secondaryBlinkActive = true;
    secondaryBlinkColor[0] = color1;
    secondaryBlinkInterval = interval;
    secondaryBlinkColorCount = 1;
    secondaryBlinkCount = 0;
    secondaryBlinkTotalCount = count;
    secondaryBlinkInterruptMain = true; // Устанавливаем флаг прерывания основного мигания
    mainBlinkPaused = true; // Ставим основное мигание на паузу
    mainBlinkPausedMillis = millis(); // Запоминаем время паузы основного мигания
  } else {
    secondaryBlinkActive = false;
    secondaryBlinkInterruptMain = false; // Сбрасываем флаг прерывания основного мигания
    mainBlinkPaused = false; // Убираем основное мигание с паузы
    blinkColors[0] = color1;
    blinkInterval = interval;
    blinkColorCount = 1;
    blinkMillis = millis();
    blinkCount = 0;
    blinkState = LOW;
    mainBlinkActive = true; // Включаем основное мигание
    mainBlinkColor = color1; // Запоминаем цвет основного мигания
  }
}

// Функция для мигания двумя светодиодами по очереди
void Leds::blink(LEDColor color1, LEDColor color2, unsigned long interval, int count) {
  if (count > 0) {
    secondaryBlinkActive = true;
    secondaryBlinkColor[0] = color1;
    secondaryBlinkColor[1] = color2;
    secondaryBlinkInterval = interval;
    secondaryBlinkColorCount = 2;
    secondaryBlinkCount = 0;
    secondaryBlinkTotalCount = count;
    secondaryBlinkInterruptMain = true;
    mainBlinkPaused = true;
    mainBlinkPausedMillis = millis();
  } else {
    secondaryBlinkActive = false;
    secondaryBlinkInterruptMain = false;
    mainBlinkPaused = false;
    blinkColors[0] = color1;
    blinkColors[1] = color2;
    blinkInterval = interval;
    blinkColorCount = 2;
    blinkTotalCount = 0;
    blinkMillis = millis();
    blinkCount = 0;
    blinkState = LOW;
    mainBlinkActive = true; // Добавляем эту строку
    mainBlinkColor = color1;
  }
}

// Функция для мигания тремя светодиодами по очереди
void Leds::blink(LEDColor color1, LEDColor color2, LEDColor color3, unsigned long interval, int count) {
  if (count > 0) {
    secondaryBlinkActive = true;
    secondaryBlinkColor[0] = color1;
    secondaryBlinkColor[1] = color2;
    secondaryBlinkColor[2] = color3;
    secondaryBlinkInterval = interval;
    secondaryBlinkColorCount = 3;
    secondaryBlinkCount = 0;
    secondaryBlinkTotalCount = count;
    secondaryBlinkInterruptMain = true;
    mainBlinkPaused = true;
    mainBlinkPausedMillis = millis();
  } else {
    secondaryBlinkActive = false;
    secondaryBlinkInterruptMain = false;
    mainBlinkPaused = false;
    blinkColors[0] = color1;
    blinkColors[1] = color2;
    blinkColors[2] = color3;
    blinkInterval = interval;
    blinkColorCount = 3;
    blinkTotalCount = 0;
    blinkMillis = millis();
    blinkCount = 0;
    blinkState = LOW;
    mainBlinkActive = true; // Добавляем эту строку
    mainBlinkColor = color1;
  }
}

// Функция для включения светодиода
void Leds::on(LEDColor color, unsigned long duration) {
  onColor = color;
  onDuration = duration;
  isOn = true;
  onMillis = millis();
  digitalWrite(getColorPin(color), HIGH);
  secondaryBlinkActive = false;
  mainBlinkActive = false; // Выключаем основное мигание
  secondaryBlinkInterruptMain = false; // Сбрасываем флаг прерывания основного мигания
  mainBlinkPaused = false; // Убираем основное мигание с паузы
}

// Функция для выключения светодиода
void Leds::off(LEDColor color, bool all) {
  if (all) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    blinkColorCount = 0;
    secondaryBlinkActive = false;
    isOn = false;
    mainBlinkActive = false; // Выключаем основное мигание
    secondaryBlinkInterruptMain = false; // Сбрасываем флаг прерывания основного мигания
    mainBlinkPaused = false; // Убираем основное мигание с паузы
  } else {
    digitalWrite(getColorPin(color), LOW);
  }
}

// Функция для получения пина светодиода по цвету
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
  // Обработка основного мигания
  if (mainBlinkActive && blinkColorCount > 0 && !secondaryBlinkInterruptMain && !mainBlinkPaused) {
    if (millis() - blinkMillis >= blinkInterval) {
      blinkMillis = millis();
      blinkState = !blinkState;

      int colorIndex = blinkCount % blinkColorCount;

      if (blinkState == HIGH) {
        digitalWrite(getColorPin(blinkColors[colorIndex]), HIGH);
      } else {
        digitalWrite(getColorPin(blinkColors[colorIndex]), LOW);
      }

      blinkCount++;

      // Проверяем, достигнуто ли заданное количество миганий
      if (blinkTotalCount > 0 && blinkCount / blinkColorCount >= blinkTotalCount) {
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
  if (isOn) {
    if (onDuration > 0 && millis() - onMillis >= onDuration) {
      digitalWrite(getColorPin(onColor), LOW);
      isOn = false;
    }
  }

  // Обработка вторичного мигания
  if (secondaryBlinkActive) {
    if (millis() - secondaryBlinkMillis >= secondaryBlinkInterval) {
      secondaryBlinkMillis = millis();

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
  }

  // Обработка паузы основного мигания
  if (mainBlinkPaused && millis() - mainBlinkPausedMillis >= secondaryBlinkInterval) {
    mainBlinkPaused = false;
  }
}
