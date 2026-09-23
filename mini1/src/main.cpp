#include <Arduino.h>
#include <cstdint>

enum Mode { Mode1, Mode2, Mode3 };

// Мета: після спостережень класифікувати світло простим порогом.
// Підключення: 3V3 -- LDR -- GPIO4 -- 10 kOhm -- GND.
// ADC_PIN — номер GPIO, а не номер контакту роз'єму; uint8_t достатньо.
constexpr uint8_t ADC_PIN = 4;
constexpr uint8_t RELE_PIN = 2;
constexpr uint8_t BOOT_BUTTON_PIN = 0;
// uint8_t достатньо для числа 12, яке задає розрядність результату.
constexpr uint8_t ADC_BITS = 12;
// uint32_t відповідає типу параметра швидкості Serial у baud.
constexpr uint32_t SERIAL_BAUD = 115200;
// uint32_t відповідає типу мілісекунд для інтервалів вимірювань.
constexpr uint32_t SAMPLE_DELAY_MS = 200;
constexpr uint32_t DEBOUNCE_MILLIS = 50;
// Замініть 1800 серединою між ВАШИМИ типовими темним і світлим кодами.
// uint16_t відповідає діапазону 12-бітного коду ADC.
constexpr uint16_t LIGHT_THRESHOLD_RAW = 1800;

static unsigned long lastButtonActionMillis = 0;
static unsigned long lastSampleMillis = 0;
static int lastBootButtonState = HIGH;
static Mode currentMode = Mode1;

static void mode1(unsigned long currentMillis);

static void mode2();

static void mode3();

static void resetRelayState() {
    lastSampleMillis = 0;
    digitalWrite(RELE_PIN, LOW);
}

// setup() виконується один раз після ввімкнення або Reset.
void setup() {
    // Serial.begin(baud) відкриває Serial із заданою швидкістю.
    Serial.begin(SERIAL_BAUD);
    // analogReadResolution(bits) задає розрядність поверненого коду.
    analogReadResolution(ADC_BITS);
    // Функція приймає режим attenuation і нічого не повертає.
    analogSetAttenuation(ADC_11db);
    pinMode(RELE_PIN, OUTPUT);
    pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);
}

// loop() перевіряє стан кнопки та виконує обраний режим без блокування через delay.
void loop() {
    const unsigned long currentMillis = millis();

    if (currentMillis - lastButtonActionMillis >= DEBOUNCE_MILLIS) {
        const int currentBootButtonState = digitalRead(BOOT_BUTTON_PIN);
        if (currentBootButtonState == LOW && lastBootButtonState == HIGH) {
            Serial.print("Boot button state pressed\n");
            lastButtonActionMillis = currentMillis;
            resetRelayState();
            if (currentMode == Mode1) {
                currentMode = Mode2;
                Serial.print("Turn on mode 2\n");
            } else if (currentMode == Mode2) {
                currentMode = Mode3;
                Serial.print("Turn on mode 3\n");
            } else {
                currentMode = Mode1;
                Serial.print("Turn on mode 1\n");
            }
        }
        lastBootButtonState = currentBootButtonState;
    }

    switch (currentMode) {
        case Mode1:
            mode1(currentMillis);
            break;
        case Mode2:
            mode2();
            break;
        case Mode3:
            mode3();
            break;
    }
}

// Режим 1: автоматичне керування реле за датчиком освітлення (LDR).
static void mode1(const unsigned long currentMillis) {
    if (currentMillis - lastSampleMillis >= SAMPLE_DELAY_MS) {
        lastSampleMillis = currentMillis;

        const uint16_t raw = analogRead(ADC_PIN);
        const bool isBright = raw >= LIGHT_THRESHOLD_RAW;

        Serial.print("raw=");
        Serial.print(raw);
        Serial.print(" | light=");
        Serial.println(isBright ? "bright" : "dark");

        if (isBright) {
            digitalWrite(RELE_PIN, LOW);
        } else {
            digitalWrite(RELE_PIN, HIGH);
        }
    }
}

static void mode2() {
    Serial.print("Mode 2 (Manual On)");
    digitalWrite(RELE_PIN, LOW);
}

static void mode3() {
    Serial.print("Mode 3 (Manual Off)");
    digitalWrite(RELE_PIN, HIGH);
}
