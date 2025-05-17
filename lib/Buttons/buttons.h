#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

constexpr int buttonNextPin{ 25 };
constexpr int buttonSetPin{ 26 };
constexpr int debounceDelay{ 300 };

enum TimeSettingStage {HOURS, MINUTES, SECONDS, DONE};
extern TimeSettingStage timeStage;

extern volatile bool buttonSetPressed; 
extern int numHours;

extern int numMinutes;
extern int numSeconds;

/**
 * @brief Button Next interrupt
 * The button which increments the unit of time.
 */
void IRAM_ATTR handleButtonNext();

/**
 * @brief Button Set interrupt
 * The button which sets the current unit of time
 * and goes to the next one.
 */
void IRAM_ATTR handleButtonSet();

/**
 * @brief Buttons init
 */
void buttonsInit();

/**
 * @brief Buttons logic:
 * Configure the time after which
 * the buzzer will sound.
 */
void buttonsLogic();

#endif
