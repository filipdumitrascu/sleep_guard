#include "lcd.h"

LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

void lcdPrintMessage(const char* line1, const char* line2, int ns)
{
    lcd.clear();

    // Pint the content
    lcd.setCursor(0, 0);
    lcd.print(line1);

    lcd.setCursor(0, 1);
    lcd.print(line2);

    delay(ns);
}
