#include <GyverButton.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <SPI.h>
#include <microDS3231.h>

// Подключаем наши модули
#include "menu.h"
#include "plot.h"
#include "interrupts.h"

// Объекты
LiquidCrystal_I2C lcd(0x27, 16, 2);
File myFile;
MicroDS3231 rtc;
GButton down(4, HIGH_PULL);
GButton select_(5, HIGH_PULL);
GButton up(6, HIGH_PULL);

// Глобальные переменные
#define MIN_PERIOD 15000
#define MAX_PERIOD 60000
#define LOG_PERIOD 1000   // пример периода логирования, можно подставить своё
int menu = 1;
int plot_array[20];
int pinCS = 10;
int max_value = -32000;
int min_value = 32000;
int value;
unsigned int multiplier;
unsigned long counts;
unsigned long ipm;
unsigned long previousMillis;
float mcrZv = 0.0;

void setup() {
  rtc.setTime(SEC, MIN, HOUR, 24, 12, 2022);
  multiplier = MAX_PERIOD / MIN_PERIOD;
  counts = 0;
  ipm = 0;

  Serial.begin(9600);
  pinMode(pinCS, OUTPUT);
  pinMode(2, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();

  attachInterrupt(0, tube_impulse, FALLING);

  initPlot();      // из plot.cpp
  updateMenu();    // из menu.cpp

  if (SD.begin()) {
    Serial.println("SD card is ready to use.");
  } else {
    Serial.println("SD card initialization failed");
    return;
  }

  Serial.print("                      уд/мин");
  Serial.print("    ");
  Serial.print("мкЗв");
}

void loop() {
  // Обработка кнопок
  up.tick();
  down.tick();
  ok.tick();

  // Навигация по меню
  if (up.isClick()) {
    menu--;
    updateMenu();
    while (up.isClick());
  }

  if (down.isClick()) {
    menu++;
    updateMenu();
    while (down.isClick());
  }

  if (ok.isClick()) {
    updateMenu();
    switch (menu) {
      case 1: Tab_rad(); break;
      case 2: Tab_graph(); break;
      case 3: Tab_time(); break;
    }
  }

  // Логирование данных каждые LOG_PERIOD миллисекунд
  if (millis() - previousMillis > LOG_PERIOD) {
    previousMillis = millis();
    ipm = counts * multiplier;
    mcrZv = ipm / 151.0;

    myFile = SD.open("rad.txt", FILE_WRITE);
    if (myFile) {
      Serial.print(rtc.getDateString());
      Serial.print(" ");
      Serial.print(rtc.getTimeString());
      Serial.print("\t");
      Serial.print(ipm);
      Serial.print("      ");
      Serial.print(mcrZv);
      Serial.print("\r\n");

      myFile.print(rtc.getDateString());
      myFile.print(" ");
      myFile.print(rtc.getTimeString());
      myFile.print("\t");
      myFile.print(ipm);
      myFile.print("    ");
      myFile.print(mcrZv);
      myFile.print("\r\n");
      myFile.close();
    }

    value = map(ipm, 0, 72, 10, 80);

    for (byte i = 0; i < 14; i++) {
      if (plot_array[i] > max_value) max_value = plot_array[i];
      if (plot_array[i] < min_value) min_value = plot_array[i];
    }

    counts = 0;
  }
}
