#include <Arduino.h>

enum Mode { Undefined, Mode1, Mode2 };

constexpr int LED_BLUE_PIN = 15;
constexpr int LED_RED_PIN = 16;
constexpr int BUTTON_PIN = 21;
constexpr int BOOT_BUTTON_PIN = 0;
constexpr int DEBOUNCE_MILLIS = 50;

constexpr int DELAY_ACTION_BUTTON = 200;
constexpr int DELAY_BOOT_BUTTON = 1000;

static unsigned long lastButtonActionMillis = 0;
static unsigned long lastLedActionMillis = 0;

static Mode currentMode = Undefined;

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(LED_BLUE_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(BOOT_BUTTON_PIN, INPUT);
}

static void mode1(unsigned long currentMillis);

static void mode2(unsigned long currentMillis);

static void resetLedsState() {
    lastLedActionMillis = 0;
    digitalWrite(LED_BLUE_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);
}

void loop() {
    const unsigned long currentMillis = millis();

    if (currentMillis - lastButtonActionMillis >= DEBOUNCE_MILLIS) {
        if (digitalRead(BUTTON_PIN) == LOW && currentMode != Mode1) {
            Serial.print("External button state pressed");
            lastButtonActionMillis = currentMillis;
            resetLedsState();
            Serial.print("Turn on mode 1\n");
            currentMode = Mode1;
        } else if (digitalRead(BOOT_BUTTON_PIN) == LOW && currentMode != Mode2) {
            Serial.print("Boot button state pressed");
            lastButtonActionMillis = currentMillis;
            resetLedsState();
            Serial.print("Turn on mode 2\n");
            currentMode = Mode2;
        }
    }

    switch (currentMode) {
        case Mode1:
            mode1(currentMillis);
            break;
        case Mode2:
            mode2(currentMillis);
            break;
        case Undefined: break;
    }
}

static void invertLedState(const int ledPin) {
    digitalWrite(ledPin, digitalRead(ledPin) ^ 1);
}

static void mode1(const unsigned long currentMillis) {
    if (currentMillis - lastLedActionMillis > DELAY_ACTION_BUTTON) {
        lastLedActionMillis = currentMillis;
        invertLedState(LED_BLUE_PIN);
        invertLedState(LED_RED_PIN);
    }
}

static void mode2(const unsigned long currentMillis) {
    if (currentMillis - lastLedActionMillis > DELAY_BOOT_BUTTON) {
        lastLedActionMillis = currentMillis;
        invertLedState(LED_BLUE_PIN);
        digitalWrite(LED_RED_PIN, digitalRead(LED_BLUE_PIN) ^ 1);
    }
}
