#ifndef SENSORS_H
#define SENSORS_H

// Функция для чтения значения датчика
int readSensor();

// Функция для проверки, превышает ли датчик порог
bool isSensorTriggered(int threshold);

#endif
