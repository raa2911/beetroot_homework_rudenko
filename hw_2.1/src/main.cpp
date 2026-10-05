#include <Arduino.h>
#include <cstdint>

enum class LedState : uint8_t { Off, On };
enum class LedMode : uint8_t { Blink, AlwaysOn, AlwaysOff };

constexpr uint8_t LED_PIN = 2;
constexpr uint8_t BUTTON_PIN = 0;
constexpr uint32_t BLINK_INTERVAL_MS = 500;
constexpr uint32_t DEBOUNCE_MS = 30;
constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint32_t STATS_EVERY_ITERATIONS = 1000;

static volatile bool buttonPressed = false;

static void IRAM_ATTR onButtonPressed() {
    buttonPressed = true;
}

static void ledSet(const LedState state) {
    digitalWrite(LED_PIN, state == LedState::On ? HIGH : LOW);
}

static void ledInit() {
    pinMode(LED_PIN, OUTPUT);
    ledSet(LedState::Off);
}

static bool buttonClicked(const uint32_t nowMs) {
    static bool debouncing = false;
    static uint32_t edgeMs = 0;

    if (buttonPressed) {
        buttonPressed = false;
        if (!debouncing) {
            debouncing = true;
            edgeMs = nowMs;
        }
    }
    if (debouncing && nowMs - edgeMs >= DEBOUNCE_MS) {
        debouncing = false;
        return digitalRead(BUTTON_PIN) == LOW;
    }
    return false;
}

static LedMode nextMode(const LedMode mode) {
    switch (mode) {
        case LedMode::Blink:
            return LedMode::AlwaysOn;
        case LedMode::AlwaysOn:
            return LedMode::AlwaysOff;
        default:
            return LedMode::Blink;
    }
}

static const char* modeName(const LedMode mode) {
    switch (mode) {
        case LedMode::Blink:
            return "Blink";
        case LedMode::AlwaysOn:
            return "AlwaysOn";
        default:
            return "AlwaysOff";
    }
}

static void applyMode(const LedMode mode) {
    if (mode == LedMode::AlwaysOn) {
        ledSet(LedState::On);
    } else if (mode == LedMode::AlwaysOff) {
        ledSet(LedState::Off);
    }
    Serial.printf("mode -> %s\n", modeName(mode));
}

static void blink(const uint32_t nowMs) {
    static uint32_t lastToggleMs = 0;
    static LedState state = LedState::Off;

    if (nowMs - lastToggleMs >= BLINK_INTERVAL_MS) {
        lastToggleMs = nowMs;
        state = state == LedState::On ? LedState::Off : LedState::On;
        ledSet(state);
    }
}

static void recordLoopTime(const uint32_t durationUs) {
    static uint32_t count = 0;
    static uint32_t sumUs = 0;
    static uint32_t minUs = UINT32_MAX;
    static uint32_t maxUs = 0;

    ++count;
    sumUs += durationUs;
    minUs = min(minUs, durationUs);
    maxUs = max(maxUs, durationUs);

    if (count >= STATS_EVERY_ITERATIONS) {
        Serial.printf("loop: avg=%.2fus min=%luus max=%luus\n",
                      static_cast<float>(sumUs) / count,
                      static_cast<unsigned long>(minUs), static_cast<unsigned long>(maxUs));
        count = 0;
        sumUs = 0;
        minUs = UINT32_MAX;
        maxUs = 0;
    }
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    ledInit();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonPressed, FALLING);
    applyMode(LedMode::Blink);
}

void loop() {
    static LedMode mode = LedMode::Blink;

    const uint32_t startUs = micros();
    const uint32_t nowMs = millis();

    if (buttonClicked(nowMs)) {
        mode = nextMode(mode);
        applyMode(mode);
    }
    if (mode == LedMode::Blink) {
        blink(nowMs);
    }

    recordLoopTime(micros() - startUs);
}
