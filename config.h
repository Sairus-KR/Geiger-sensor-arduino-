// config.h
#ifndef CONFIG_H
#define CONFIG_H

// --- Пины ---
const int buttonPin1 = 2;
const int buttonPin2 = 3;
const int ledPin     = 13;
const int sensorPin  = A0;

// --- Константы программы ---
const int debounceDelay = 50;  // защита от дребезга кнопки
const int sensorThreshold = 500; // порог датчика

#endif
