# Модуль 2.1 — Embedded C++: неблокуючий blink, кнопка з перериванням

Плата: **ESP32-S3-DevKitC-1 (N16R8)**, framework Arduino, PlatformIO. Весь код — `src/main.cpp`.

## Підключення

| Елемент | GPIO | Схема |
|---|---|---|
| LED | GPIO2 | GPIO2 → резистор 220–330 Ω → анод LED, катод → GND |
| Кнопка | GPIO0 | вбудована кнопка **BOOT** (замикає на GND, внутрішній pull-up) |

## Як виконано вимоги

1. **Blink:** `enum class LedState`, `constexpr` пін та інтервал, `ledInit()` / `ledSet(LedState)`,
   неблокуючий blink на `millis()` без `delay()`.
2. **Параметри:** усі числа винесені в `constexpr`-константи.
3. **Час superloop:** `micros()` на початку та в кінці `loop()`, кожні 1000 ітерацій
   у Serial друкуються avg/min/max.
4. **Кнопка:** `attachInterrupt(…, FALLING)`, ISR `IRAM_ATTR` лише ставить `volatile bool buttonPressed`.
   Антидребезг (вікно 30 мс + перечитування піну) і перемикання
   `Blink → AlwaysOn → AlwaysOff → Blink` виконуються в `loop()`.

Глобальний лише прапорець ISR. Решта стану — `static` локальні змінні функцій.
Немає `new`/`malloc`, `String` і контейнерів STL.

## Збірка

```
pio run -t upload && pio device monitor
```

## Результат виконання

<!-- Вставте сюди вивід Serial Monitor і фото/відео плати -->
