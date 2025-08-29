#include "Arduino.h"
#include "config.h"
#include "sensors.h"

// Читаем аналоговый датчик
int readSensor() {
    return analogRead(sensorPin);
}

// Проверяем, превысил ли датчик заданный порог
bool isSensorTriggered(int threshold) {
    return readSensor() > threshold;
}
