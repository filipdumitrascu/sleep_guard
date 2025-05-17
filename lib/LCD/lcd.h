#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27

extern LiquidCrystal_I2C lcd;

/**
 * @brief Prints a message on the 2 lines 
 * (maximum 16 chars per line)
 * 
 * @param line1 first line
 * @param line2 second line
 * @param ns how many nanoseconds the message 
 * stays at least on the display
 */
void lcdPrintMessage(const char* line1, const char* line2, int ns);

#endif
