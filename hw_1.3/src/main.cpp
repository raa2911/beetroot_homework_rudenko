#include <Arduino.h>
#include <TimeLib.h>

constexpr int LED_BLUE = 42;
constexpr int LED_RED = 20;

int lastSecond = -1;

void setup() {
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    int currentSecond = second();
    if (currentSecond != lastSecond) {
        lastSecond = currentSecond;
        if (currentSecond % 2 == 0) {
            digitalWrite(LED_BLUE, HIGH);
            digitalWrite(LED_RED, LOW);
            Serial.print("Second: ");
            Serial.print(currentSecond);
            Serial.println(" - LED ON");
        } else {
            digitalWrite(LED_BLUE, LOW);
            digitalWrite(LED_RED, HIGH);
            Serial.print("Second: ");
            Serial.print(currentSecond);
            Serial.println(" - LED OFF");
        }
    }
}
