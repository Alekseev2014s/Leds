#include "Leds.h"

unsigned long Leds::blinkMillis = 0;
int Leds::blinkCount = 0;
int Leds::blinkTotalCount = 0;
LEDColor Leds::blinkColors[3] = {LEDColor::RED, LEDColor::BLUE, LEDColor::GREEN};
unsigned long Leds::blinkInterval = 500;
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
unsigned long Leds::secondaryBlinkInterval = 500;
int Leds::secondaryBlinkColorCount = 0;
bool Leds::mainBlinkActive = false;
bool Leds::secondaryBlinkInterruptMain = false;
bool Leds::mainBlinkPaused = false;
unsigned long Leds::mainBlinkPausedMillis = 0;
LEDColor Leds::mainBlinkColor = LEDColor::RED;
int Leds::redPin = RED_LED_PIN;
int Leds::bluePin = BLUE_LED_PIN;
int Leds::greenPin = GREEN_LED_PIN;

void Leds::begin(const int redPin, const int greenPin, const int bluePin) {
  Leds::redPin = redPin;
  Leds::bluePin = bluePin;
  Leds::greenPin = greenPin;

  pinMode(Leds::redPin, OUTPUT);
  pinMode(Leds::bluePin, OUTPUT);
  pinMode(Leds::greenPin, OUTPUT);
  digitalWrite(Leds::redPin, LOW);
  digitalWrite(Leds::bluePin, LOW);
  digitalWrite(Leds::greenPin, LOW);
}

void Leds::setInterval(const unsigned long time) {
  blinkInterval = time;
}

void Leds::blink(std::initializer_list<LEDColor> colors, unsigned long interval, int count) {
  secondaryBlinkActive = count > 0;
  secondaryBlinkInterval = interval;
  secondaryBlinkCount = 0;
  secondaryBlinkTotalCount = count;
  secondaryBlinkInterruptMain = secondaryBlinkActive;
  mainBlinkPaused = secondaryBlinkActive;
  mainBlinkPausedMillis = millis();

  int colorIndex = 0;
  for (LEDColor color : colors) {
    if (secondaryBlinkActive) {
      secondaryBlinkColor[colorIndex] = color;
    } else {
      blinkColors[colorIndex] = color;
    }
    colorIndex++;
  }

  if (secondaryBlinkActive) {
    secondaryBlinkColorCount = colorIndex;
  } else {
    blinkInterval = interval;
    blinkColorCount = colorIndex;
    blinkTotalCount = 0;
    blinkMillis = millis();
    blinkCount = 0;
    mainBlinkActive = true;
    mainBlinkColor = *colors.begin();
  }

  digitalWrite(redPin, LOW);
  digitalWrite(bluePin, LOW);
  digitalWrite(greenPin, LOW);
}

void Leds::on(const LEDColor color, const unsigned long duration) {
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

void Leds::off(const LEDColor color, const bool all) {
  if (all) {
    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, LOW);
    digitalWrite(greenPin, LOW);
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

int Leds::getColorPin(const LEDColor color) {
  switch (color) {
    case RED:
      return redPin;
    case BLUE:
      return bluePin;
    case GREEN:
      return greenPin;
    default:
      return redPin;
  }
}

void Leds::tick() {
  const unsigned long currentMillis = millis();

  if (mainBlinkActive && blinkColorCount > 0 && !secondaryBlinkInterruptMain && !mainBlinkPaused) {
    if (currentMillis - blinkMillis >= blinkInterval) {
      blinkMillis = currentMillis;

      const int colorIndex = blinkCount % blinkColorCount;

      digitalWrite(getColorPin(blinkColors[colorIndex]), !digitalRead(getColorPin(blinkColors[colorIndex])));

      if (digitalRead(getColorPin(blinkColors[colorIndex])) == LOW) {
        blinkCount++;
      }

      if (blinkTotalCount > 0 && blinkCount >= blinkTotalCount) {
        mainBlinkActive = false;
        off(RED, true);
        blinkCount = 0;
        blinkTotalCount = 0;
      } else {
        if (blinkCount >= blinkColorCount) {
          blinkCount = 0;
        }
      }
    }
  }

  if (isOn && onDuration > 0 && currentMillis - onMillis >= onDuration) {
    digitalWrite(getColorPin(onColor), LOW);
    isOn = false;
  }

  if (secondaryBlinkActive && currentMillis - secondaryBlinkMillis >= secondaryBlinkInterval) {
    secondaryBlinkMillis = currentMillis;

    int colorIndex = secondaryBlinkCount % secondaryBlinkColorCount;

    digitalWrite(getColorPin(secondaryBlinkColor[colorIndex]), !digitalRead(getColorPin(secondaryBlinkColor[colorIndex])));

    if (digitalRead(getColorPin(secondaryBlinkColor[colorIndex])) == LOW) {
      secondaryBlinkCount++;
    }

    if (secondaryBlinkTotalCount > 0 && secondaryBlinkCount >= secondaryBlinkTotalCount) {
      secondaryBlinkActive = false;
      secondaryBlinkCount = 0;
      secondaryBlinkInterruptMain = false;
      digitalWrite(redPin, LOW);
      digitalWrite(bluePin, LOW);
      digitalWrite(greenPin, LOW);
    }
  }

  if (mainBlinkPaused && currentMillis - mainBlinkPausedMillis >= secondaryBlinkInterval) {
    mainBlinkPaused = false;
  }
}
