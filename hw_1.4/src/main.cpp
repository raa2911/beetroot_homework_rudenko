#include <Arduino.h>
#include <TimeLib.h>

constexpr int LED_PIN = 10;

int lastSecond = -1;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    int currentSecond = second();
    if (currentSecond != lastSecond) {
        lastSecond = currentSecond;
        if (currentSecond % 2 == 0) {
            digitalWrite(LED_PIN, HIGH);
            Serial.print("Second: ");
            Serial.print(currentSecond);
            Serial.println(" - LED ON");
        } else {
            digitalWrite(LED_PIN, LOW);
            Serial.print("Second: ");
            Serial.print(currentSecond);
            Serial.println(" - LED OFF");
        }
    }
}
