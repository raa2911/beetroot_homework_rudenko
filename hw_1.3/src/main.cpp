#include <Arduino.h>
#include <TimeLib.h>

constexpr int LED_BLUE_PIN = 15;
constexpr int LED_RED_PIN = 16;
constexpr int BUTTON_PIN = 16;

int lastSecond = -1;

void setup() {
    pinMode(LED_BLUE_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);
    Serial.begin(115200);
}

void mode1();

void loop() {
    mode1();
}

void mode1() {
    int currentSecond = second();
    if (currentSecond != lastSecond) {
        lastSecond = currentSecond;
        if (currentSecond % 2 == 0) {
            digitalWrite(LED_BLUE_PIN, HIGH);
            digitalWrite(LED_RED_PIN, LOW);
            Serial.print("Second: ");
            Serial.print(currentSecond);
            Serial.println(" - LED ON");
        } else {
            digitalWrite(LED_BLUE_PIN, LOW);
            digitalWrite(LED_RED_PIN, HIGH);
            Serial.print("Second: ");
            Serial.print(currentSecond);
            Serial.println(" - LED OFF");
        }
    }
}
