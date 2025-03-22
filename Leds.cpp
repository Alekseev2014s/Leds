#include "Leds.h"

unsigned long Leds::currentMillis = 0;
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
    BlinkConfig* blinkConfig = &mainBlink;
    if (count > 0) {
        secondaryBlink.active = true;
        secondaryBlink.interval = interval;
        secondaryBlink.totalCount = count;
        secondaryBlinkInterruptMain = true;
        mainBlinkPaused = true;
        mainBlinkPausedMillis = millis();
        blinkConfig = &secondaryBlink;
    } else {
        mainBlink.interval = interval;
        mainBlink.totalCount = 0;
        mainBlink.millis = millis();
        mainBlink.active = true;
    }

    int colorIndex = 0;
    for (const LEDColor color : colors) {
        blinkConfig->colors[colorIndex++] = color;
    }
    blinkConfig->colorCount = colorIndex;
    blinkConfig->count = 0;

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

bool Leds::hasElapsed(const unsigned long startMillis, const unsigned long duration) {
    return currentMillis - startMillis >= duration;
}

void Leds::tick() {
    currentMillis = millis();

    if (isOn) {
        if (hasElapsed(onMillis, onDuration)) {
            digitalWrite(getColorPin(onColor), !ledOnLevel);
            isOn = false;
        }
        return;
    }

    if (mainBlinkPaused && hasElapsed(mainBlinkPausedMillis, secondaryBlink.interval)) {
        mainBlinkPaused = false;
    }

    if (mainBlink.active && mainBlink.colorCount > 0 && !secondaryBlinkInterruptMain && !mainBlinkPaused) {
        handleBlink(mainBlink);
    }

    if (secondaryBlink.active) {
        handleBlink(secondaryBlink);
        if (!secondaryBlink.active) {
            secondaryBlinkInterruptMain = false;
        }
    }
}

void Leds::handleBlink(BlinkConfig &blinkConfig) {
    if (hasElapsed(blinkConfig.millis, blinkConfig.interval)) {
        blinkConfig.millis = currentMillis;
        blinkConfig.state = !blinkConfig.state;

        const int colorIndex = blinkConfig.count % blinkConfig.colorCount;
        setLedState(blinkConfig.colors[colorIndex], blinkConfig.state);

        if (!blinkConfig.state) {
            blinkConfig.count++;
        }

        if (blinkConfig.totalCount > 0 && blinkConfig.count >= blinkConfig.totalCount) {
            blinkConfig.active = false;
            turnOffAllLedPins();
            blinkConfig.count = 0;
            blinkConfig.totalCount = 0;
        }
    }
}

void Leds::setLedState(const LEDColor color, const bool state) {
    digitalWrite(getColorPin(color), state ? ledOnLevel : !ledOnLevel);
}

