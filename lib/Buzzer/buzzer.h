#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

constexpr int buzzerPin{ 27 };
constexpr int buzzerFrequency{ 1000 };

/**
 * @brief Timer interrupt.
 */
void IRAM_ATTR scanISR();

/**
 * @brief Initialize buzzer.
 */
void buzzerInit();

/**
 * @brief Set the alarm. 
 * (with the time configured by the buttons)
 */
void setAlarm();

/**
 * @brief Buzzer logic:
 * Rings when the timer is up.
 */
void buzzerLogic();

#endif
