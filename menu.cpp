#include <LiquidCrystal_I2C.h>
#include "menu.h"
#include "plot.h"   // для функций графика
#include "main.h"   // если нужны глобальные переменные, можно создать отдельный header с ними

extern LiquidCrystal_I2C lcd;
extern int menu;
extern unsigned long ipm;
extern float mcrZv;
extern int value;
extern int plot_array[20];
extern int max_value;
extern int min_value;
extern GButton ok;

void updateMenu() {
  switch (menu) {
    case 0: menu = 1; break;
    case 1:
      lcd.clear();
      lcd.print(">radiation");
      lcd.setCursor(0, 1);
      lcd.print(" graphic");
      break;
    case 2:
      lcd.clear();
      lcd.print(" radiation");
      lcd.setCursor(0, 1);
      lcd.print(">graphic");
      break;
    case 3:
      lcd.clear();
      lcd.print(" graphic");
      lcd.setCursor(0, 1);
      lcd.print(">time");
      break;
    case 4: menu = 3; break;
  }
}

void Tab_rad() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(ipm);
  lcd.setCursor(3, 0);
  lcd.print("imp/min");
  lcd.setCursor(0, 1);
  lcd.print(mcrZv);
  lcd.setCursor(5, 1);
  lcd.print("mcrZv");

  if (mcrZv >= 0 && mcrZv <= 0.2) {
    lcd.setCursor(11, 1);
    lcd.print("norm");
  } else if (mcrZv >= 0.2 && mcrZv <= 0.6) {
    lcd.setCursor(11, 1);
    lcd.print("risk!");
  } else if (mcrZv >= 0.6) {
    lcd.setCursor(11, 1);
    lcd.print("death");
  }

  ok.tick();
  if (ok.isClick()) return;
}

void Tab_graph() {
  lcd.clear();
  drawPlot(0, 1, 16, 2, 10, 80, value);
  lcd.setCursor(14, 0);
  lcd.print(max_value); lcd.print(" ");
  lcd.setCursor(14, 1);
  lcd.print(min_value); lcd.print(" ");
  ok.tick();
  if (ok.isClick()) return;
}

void Tab_time() {
  lcd.clear();
  // предполагаем, что есть глобальный объект rtc
  extern MicroDS3231 rtc;
  lcd.setCursor(3, 0);
  lcd.print(rtc.getDateString());
  lcd.setCursor(3, 1);
  lcd.print(rtc.getTimeString());
  ok.tick();
  if (ok.isClick()) return;
}
