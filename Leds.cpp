#include "Leds.h"

unsigned long Leds::onMillis = 0;
bool Leds::isOn = false;
LEDColor Leds::onColor = LEDColor::RED;
unsigned long Leds::onDuration = 0;
bool Leds::secondaryBlinkInterruptMain = false;
bool Leds::mainBlinkPaused = false;
unsigned long Leds::mainBlinkPausedMillis = 0;
int Leds::redPin = RED_LED_PIN;
int Leds::bluePin = BLUE_LED_PIN;
int Leds::greenPin = GREEN_LED_PIN;
int Leds::ledOnLevel = HIGH;

BlinkConfig Leds::mainBlink;
BlinkConfig Leds::secondaryBlink;

void Leds::begin(const int redPin, const int greenPin, const int bluePin) {
  Leds::redPin = redPin;
  Leds::bluePin = bluePin;
  Leds::greenPin = greenPin;

  pinMode(Leds::redPin, OUTPUT);
  pinMode(Leds::bluePin, OUTPUT);
  pinMode(Leds::greenPin, OUTPUT);
  turnOffAllLedPins();
}

void Leds::setOnLevel(const int level) {
  ledOnLevel = level;
}

void Leds::setInterval(const unsigned long time) {
  mainBlink.interval = time;
}

void Leds::turnOffAllLedPins() {
  digitalWrite(redPin, !ledOnLevel);
  digitalWrite(bluePin, !ledOnLevel);
  digitalWrite(greenPin, !ledOnLevel);
}

void Leds::blink(const std::initializer_list<LEDColor> colors, const int count, const unsigned long interval) {
  secondaryBlink.active = count > 0;
  secondaryBlink.interval = interval;
  secondaryBlink.count = 0;
  secondaryBlink.totalCount = count;
  secondaryBlinkInterruptMain = secondaryBlink.active;
  mainBlinkPaused = secondaryBlink.active;
  mainBlinkPausedMillis = millis();

  int colorIndex = 0;
  for (LEDColor color : colors) {
    if (secondaryBlink.active) {
      secondaryBlink.colors[colorIndex] = color;
    } else {
      mainBlink.colors[colorIndex] = color;
    }
    colorIndex++;
  }

  if (secondaryBlink.active) {
    secondaryBlink.colorCount = colorIndex;
  } else {
    mainBlink.interval = interval;
    mainBlink.colorCount = colorIndex;
    mainBlink.totalCount = 0;
    mainBlink.millis = millis();
    mainBlink.count = 0;
    mainBlink.active = true;
  }

  turnOffAllLedPins();
}

void Leds::on(const LEDColor color, const unsigned long duration) {
  onColor = color;
  onDuration = duration;
  isOn = true;
  onMillis = millis();
  turnOffAllLedPins();
  digitalWrite(getColorPin(color), ledOnLevel);
}

void Leds::off() {
  turnOffAllLedPins();
  mainBlink.colorCount = 0;
  secondaryBlink.active = false;
  isOn = false;
  mainBlink.active = false;
  secondaryBlinkInterruptMain = false;
  mainBlinkPaused = false;
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

  if (isOn && onDuration > 0 && currentMillis - onMillis >= onDuration) {
    digitalWrite(getColorPin(onColor), !ledOnLevel);
    isOn = false;
  }

  if (mainBlink.active && mainBlink.colorCount > 0 && !secondaryBlinkInterruptMain && !mainBlinkPaused && !isOn) {
    if (currentMillis - mainBlink.millis >= mainBlink.interval) {
      mainBlink.millis = currentMillis;
      mainBlink.state = !mainBlink.state;

      const int colorIndex = mainBlink.count % mainBlink.colorCount;

      digitalWrite(getColorPin(mainBlink.colors[colorIndex]), mainBlink.state ? ledOnLevel : !ledOnLevel);

      if (!mainBlink.state) {
        mainBlink.count++;
      }

      if (mainBlink.totalCount > 0 && mainBlink.count >= mainBlink.totalCount) {
        mainBlink.active = false;
        off();
        mainBlink.count = 0;
        mainBlink.totalCount = 0;
      } else {
        if (mainBlink.count >= mainBlink.colorCount) {
          mainBlink.count = 0;
        }
      }
    }
  }

  if (secondaryBlink.active && currentMillis - secondaryBlink.millis >= secondaryBlink.interval && !isOn) {
    secondaryBlink.millis = currentMillis;
    secondaryBlink.state = !secondaryBlink.state;

    int colorIndex = secondaryBlink.count % secondaryBlink.colorCount;

    digitalWrite(getColorPin(secondaryBlink.colors[colorIndex]), secondaryBlink.state ? ledOnLevel : !ledOnLevel);

    if (!secondaryBlink.state) {
      secondaryBlink.count++;
    }

    if (secondaryBlink.totalCount > 0 && secondaryBlink.count >= secondaryBlink.totalCount) {
      secondaryBlink.active = false;
      secondaryBlink.count = 0;
      secondaryBlinkInterruptMain = false;
      turnOffAllLedPins();
    }
  }

  if (mainBlinkPaused && currentMillis - mainBlinkPausedMillis >= secondaryBlink.interval) {
    mainBlinkPaused = false;
  }
}
