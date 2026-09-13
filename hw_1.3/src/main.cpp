#include <Arduino.h>
#include <TimeLib.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

int lastSecond = -1;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    int currentSecond = second();
    if (currentSecond != lastSecond) {
        lastSecond = currentSecond;
        if (currentSecond % 2 == 0) {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.print("Second: ");
            Serial.print(currentSecond);
            Serial.println(" - LED ON");
        } else {
            digitalWrite(LED_BUILTIN, LOW);
            Serial.print("Second: ");
            Serial.print(currentSecond);
            Serial.println(" - LED OFF");
        }
    }
}
