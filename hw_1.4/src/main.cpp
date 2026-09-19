#include <Arduino.h>
// #include <TimeLib.h>

constexpr int LED_BLUE_PIN = 15;
constexpr int LED_RED_PIN = 16;
constexpr int BUTTON_PIN = 21;
constexpr int DEBOUNCE_MILLIS = 50;

unsigned long lastActionMillis = 0;
unsigned int currentButtonState = HIGH;
unsigned int lastButtonState = HIGH;

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(LED_BLUE_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

static void mode1();

void loop() {
    mode1();
}

static void mode1() {
    currentButtonState = digitalRead(BUTTON_PIN);
    unsigned long currentMillis = millis();

    if (currentButtonState != lastButtonState && currentMillis - lastActionMillis >= DEBOUNCE_MILLIS) {
        lastActionMillis = currentMillis;
        lastButtonState = currentButtonState;

        Serial.print("currentButtonState is ");
        Serial.print(currentButtonState);
        Serial.print("\n");

        if (currentButtonState == LOW) {
            digitalWrite(LED_BLUE_PIN, HIGH);
            digitalWrite(LED_RED_PIN, HIGH);
            delay(200);
            digitalWrite(LED_BLUE_PIN, LOW);
            digitalWrite(LED_RED_PIN, LOW);
        }
    }
}
