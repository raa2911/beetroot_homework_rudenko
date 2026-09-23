#include <Arduino.h>
#include <cstdint>

// Мета: після спостережень класифікувати світло простим порогом.
// Підключення: 3V3 -- LDR -- GPIO4 -- 10 kOhm -- GND.
// ADC_PIN — номер GPIO, а не номер контакту роз'єму; uint8_t достатньо.
constexpr uint8_t ADC_PIN = 4;
constexpr uint8_t RELE_PIN = 2;
// uint8_t достатньо для числа 12, яке задає розрядність результату.
constexpr uint8_t ADC_BITS = 12;
// uint32_t відповідає типу параметра швидкості Serial у baud.
constexpr uint32_t SERIAL_BAUD = 115200;
// uint32_t відповідає типу мілісекунд, який приймає delay().
constexpr uint32_t SAMPLE_DELAY_MS = 200;
// Замініть 1800 серединою між ВАШИМИ типовими темним і світлим кодами.
// uint16_t відповідає діапазону 12-бітного коду ADC.
constexpr uint16_t LIGHT_THRESHOLD_RAW = 1800;

// setup() виконується один раз після ввімкнення або Reset.
void setup() {
    // Serial.begin(baud) відкриває Serial із заданою швидкістю.
    Serial.begin(SERIAL_BAUD);
    // analogReadResolution(bits) задає розрядність поверненого коду.
    analogReadResolution(ADC_BITS);
    // Функція приймає режим attenuation і нічого не повертає.
    analogSetAttenuation(ADC_11db);
    pinMode(RELE_PIN, OUTPUT);
}

// loop() повторно читає LDR і порівнює код із вибраним порогом.
void loop() {
    // analogRead(GPIO) виконує нове вимірювання та повертає сирий код.
    const uint16_t raw = analogRead(ADC_PIN);
    // За цієї орієнтації LDR більший код відповідає яскравішому світлу.
    // bool зберігає саме двійкове рішення: яскраво або темно.
    const bool isBright = raw >= LIGHT_THRESHOLD_RAW;

    // Serial.print(value) продовжує рядок, Serial.println(value) завершує
    // його; ці функції не повертають вимірюваного значення.
    Serial.print("raw=");
    Serial.print(raw);
    Serial.print(" | light=");
    Serial.println(isBright ? "bright" : "dark");

    if (isBright) {
        digitalWrite(RELE_PIN, LOW);
    } else {
        digitalWrite(RELE_PIN, HIGH);
    }

    // delay(ms) приймає паузу в мілісекундах і нічого не повертає.
    // Очікуємо перемикання стану під час переходу через власний поріг.
    delay(SAMPLE_DELAY_MS);
}
